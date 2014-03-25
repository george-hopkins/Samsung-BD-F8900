/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "csr_bt_api.h" 
CsrAppBtHidhSdpInfoData myinfodata;
CsrBtHidhSdpInfo        mysdpInfo;
CsrCharString           myserviceName[255];
CsrUint8                mydescriptor[255];
//#undef ALSA_PLAY
#ifdef ALSA_PLAY
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <signal.h>
#include "csr_bt_result.h"

/* Settings */
#define ALSA_PRN            "alsa: "
#define DEFAULT_DEVICE      "plughw:0,0"       /* In Atlas5 Linux platform, "plughw:0,0" is for TSADC(mic), "plughw:0,1" is for MODAC(speaker) added by matthew 2011.03.09*/
#define BUFFER_SIZE         8192               /* Size of each buffer in bytes */
#define BUFFERS             32                 /* Number of buffers */
#define PLAYBACK_THRESHOLD  24                 /* Have this many filled buffers before attempting restart */
#define BUFFER_THRESHOLD    7/8              /* Wait until this percentage of BUFFER_SIZE has been received before playback */
#define READ_FRAMES         1024               /* Try to capture this many frames (apply framesize to get size in bytes) */
#define CAPTURE_THRESHOLD   12288              /* Collect capture sample buffers of this size */
#define PERIODS             4                  /* Number of ALSA driver buffers */
#define PERIOD_SIZE         8192               /* Size of each ALSA driver buffer  */

/* Default PCM sampling settings */
#define DEFAULT_BPS                      (16)    /* Bits per sample */
#define DEFAULT_FREQ                     (48000) /* Sampling frequency */
#define DEFAULT_CHANNELS                 (2)     /* Number of channels, 1:mono, 2:stereo */



typedef struct
{
//   av2filter_t			 *filter;
   char 				 *device;
   snd_pcm_sframes_t	 framesize;
   snd_pcm_stream_t 	 stream;
   snd_pcm_t			 *handle;
   snd_pcm_hw_params_t	 *params;
   CsrUint32			  sample_rate;
   CsrUint32			  channels;

   CsrBool				  run;
   pthread_mutex_t		 mutex;
   pthread_cond_t		 wake;
   pthread_t			 thread;
   CsrUint32			  errors;
   CsrBool				  suspend;

   void 				 *buffers[BUFFERS];
   CsrUint32			  lengths[BUFFERS];
   CsrUint8 			  buf_in;
   CsrUint8 			  buf_out;
   CsrUint8 			  count;

   CsrUint8 			  *capbuf;
   CsrUint32			  capsize;

   /* pcm configuration */
   CsrUint8 			 pcm_channels;
   CsrUint16			 pcm_freq;
   CsrUint8 			 pcm_bits;

//   CsrSchedBgint			 bgint_filter;
} alsa_instance_t;

static alsa_instance_t * alsa_inst = NULL;


/////////////////////////////////////////////////
CsrAppBtHidhSdpInfoData myinfodata;
CsrBtHidhSdpInfo        mysdpInfo;
CsrCharString           myserviceName[255];
CsrUint8                mydescriptor[255];
/////////////////////////////////////////////


static CsrBool alsa_init(void **instance);
static CsrBool alsa_deinit(void **instance);
static CsrBool alsa_open(void **instance);
static CsrBool alsa_close(void **instance);
static CsrBool alsa_start(void **instance);
static CsrBool alsa_stop(void **instance);
static CsrBool alsa_play(void **instance, void *data, CsrInt32 datalen);



/**************************************************************************************************
 * getPcmSettings
 **************************************************************************************************/
static void getAlsaPcmSettings(CsrUint8 *channels, CsrUint8 *bits, CsrUint16 *freq)
{
    *channels = alsa_inst->pcm_channels;
    *freq = alsa_inst->pcm_freq;
    *bits = alsa_inst->pcm_bits;
}


/**************************************************************************************************
 * getPcmSettings
 **************************************************************************************************/
static void setAlsaPcmSettings(CsrUint8 channels, CsrUint8 bits, CsrUint16 freq)
{
      printf("setAlsaPcmSettings \n");
    alsa_inst->pcm_channels = channels;
    alsa_inst->pcm_freq = freq;
    alsa_inst->pcm_bits = bits;
}

static CsrBool compareAlsaPcmSettings(CsrUint8 channels, CsrUint8 bits, CsrUint16 freq)
{
	if((alsa_inst->pcm_channels == channels) &&
		(alsa_inst->pcm_freq == freq) &&
		(alsa_inst->pcm_bits == bits))
	{

       
		return TRUE;
	}
	return FALSE;
}


/* ALSA xrun handler */
static int alsa_xrun(alsa_instance_t *inst, int err)
{
   int res;

   inst->errors++;
   res = 0;
   switch(err)
   {
	   case -EAGAIN:
		   printf(ALSA_PRN "device has more data, error count %i\n", inst->errors);
		   snd_pcm_wait(inst->handle, 1000);
		   break;

	   case -EBADFD:
		   printf(ALSA_PRN "device not in right state, error count %i\n", inst->errors);
		   snd_pcm_prepare(inst->handle);
		   break;

	   case -EPIPE:
		   printf(ALSA_PRN "device over/under-run, error count %i\n", inst->errors);
		   snd_pcm_prepare(inst->handle);
		   snd_pcm_pause(inst->handle, 1);
		   inst->suspend = TRUE;
		   break;

	   case -ESTRPIPE:
		   printf(ALSA_PRN "device suspend error, error count %i\n", inst->errors);
		   snd_pcm_prepare(inst->handle);
		   break;

	   default:
		   printf(ALSA_PRN "playback thread, unknown error, code %i, error count %i\n",
				  res, inst->errors);
		   res = err;
		   snd_pcm_prepare(inst->handle);
		   break;
   }

   return res;
}

/* ALSA playback thread */
static void *alsa_thread_play(void *instance)
{
    alsa_instance_t *inst;
    void *data;
    CsrUint32 length;
    CsrInt32 res;
    CsrInt32 frames;
    inst = (alsa_instance_t*)instance;

    printf(ALSA_PRN "thread begin...\n");

    res = 0;
    while(inst->run)
    {
        pthread_mutex_lock(&inst->mutex);
        while(((inst->count <= 0) || inst->suspend) && (inst->run))
        {
            /* Wait for buffer to contain enough data */
            pthread_cond_wait(&inst->wake, &inst->mutex);
            inst->suspend = FALSE;
            snd_pcm_pause(inst->handle, 0);
        }
        if(inst->count > 0)
        {
            /* Get current out-buffer */
            inst->count--;
            pthread_mutex_unlock(&inst->mutex);
            data   = inst->buffers[inst->buf_out];
            length = inst->lengths[inst->buf_out];
            inst->buf_out = (inst->buf_out + 1) % BUFFERS;
        }
        else
        {
            pthread_mutex_unlock(&inst->mutex);
            data = NULL;
            length = 0;
        }

        /* If no data at this point, exit */
        if(data == NULL)
        {
            break;
        }

        /* Send data to ALSA */
        frames = length / inst->framesize;
        res = snd_pcm_writei(inst->handle,
                             data,
                             frames);

        if(res < 0)
        {
            alsa_xrun(inst, res);
        }
        else if(res != frames)
        {
            printf(ALSA_PRN "playback device did not consume all frames, sent %i, consumed %i\n",
                   frames, res);
            break;
        }
    }

    printf(ALSA_PRN "thread end...\n");

    return NULL;
}




/* Initialise alsa filter */
static CsrBool alsa_init(void **instance)
{
   alsa_instance_t *inst;
   CsrUint8 i;
   *instance = CsrPmemZalloc(sizeof(alsa_instance_t));
   inst = (alsa_instance_t*)*instance;

   /* Allocate things */
   snd_pcm_hw_params_malloc(&(inst->params));
   if(inst->params == NULL)
   {
	   printf(ALSA_PRN "could not allocate ALSA hw_params structure\n");
	   return FALSE;
   }
#ifdef DEBUG
   else
   {
	   CsrMemSet(inst->params, 0, snd_pcm_hw_params_sizeof());
	   printf(ALSA_PRN "allocated hw_params structure\n");
   }
#endif

   /* Set defaults */
   inst->device = strdup(DEFAULT_DEVICE);
   inst->stream = SND_PCM_STREAM_PLAYBACK;

   /* Allocate the ringbuffers */
   for(i=0; i<BUFFERS; i++)
   {
	   inst->buffers[i] = CsrPmemAlloc(BUFFER_SIZE);
	   inst->lengths[i] = 0;
	   CsrMemSet(inst->buffers[i], 0, BUFFER_SIZE);
   }
   inst->capbuf = CsrPmemAlloc(CAPTURE_THRESHOLD);
   inst->buf_in = 0;
   inst->buf_out = 0;
   inst->count = 0;

   setAlsaPcmSettings(0, DEFAULT_BPS, 0);


   /* Initialise transfer buffer */
   pthread_mutex_init(&inst->mutex, NULL);
   pthread_cond_init(&inst->wake, NULL);

   printf(ALSA_PRN "alsa filter initialized\n");

   return TRUE;
}

/* Deinitialise alsa filter */
static CsrBool alsa_deinit(void **instance)
{
   alsa_instance_t *inst;
   CsrUint8 i;
   inst = (alsa_instance_t*)*instance;

   /* Free members */
   if(inst->params != NULL)
   {
	   snd_pcm_hw_params_free(inst->params);
   }
   if(inst->device != NULL)
   {
	   CsrPmemFree(inst->device);
   }
   for(i=0; i<BUFFERS; i++)
   {
	   CsrPmemFree(inst->buffers[i]);
   }
   CsrPmemFree(inst->capbuf);

   CsrPmemFree(inst);
   *instance = NULL;

   return TRUE;
}

/* Open alsa with current configuration */
static CsrBool alsa_open(void **instance)
{
   alsa_instance_t *inst;
   CsrBool result;
   CsrUint32 tmp;
   CsrUint8 bits;
   CsrUint8 chans;
   CsrUint16 freq;
   inst = (alsa_instance_t*)*instance;

   printf(ALSA_PRN "opening alsa \n");

   /* Close before reopen */
   result = TRUE;
   if(inst->handle != 0)
   {
	   printf(ALSA_PRN "filter already open - reopening\n");
	   result = alsa_close(instance);
   }

   /* Get global AV2 configuration */
   getAlsaPcmSettings(&chans, &bits, &freq);
   inst->channels = chans;
   inst->sample_rate = freq;

   /* Open PCM device */
   if(result && (snd_pcm_open(&(inst->handle),
							  inst->device,
							  inst->stream,
							  0) < 0))
   {
	   printf(ALSA_PRN "could not open PCM device '%s'\n", inst->device);
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "pcm device opened, 0x%08x\n", (unsigned int )inst->handle);
	   printf(ALSA_PRN "pcm device params, 0x%08x\n", (unsigned int )inst->params);
   }


   /* Get config */
   if(result && (snd_pcm_hw_params_any(inst->handle,
									   inst->params) < 0))
   {
	   printf(ALSA_PRN "could not get hardware parameters\n");
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "got hw parameters\n");
   }



   /* Set access mode */
   if(result && (snd_pcm_hw_params_set_access(inst->handle,
											  inst->params,
											  SND_PCM_ACCESS_RW_INTERLEAVED) < 0))
   {
	   printf(ALSA_PRN "could not set access mode\n");
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "access mode set\n");
   }


   /* Set sample format (signed 16bit little endian) */
   if(result && (snd_pcm_hw_params_set_format(inst->handle,
											  inst->params,
#ifndef __BIG_ENDIAN__
											  SND_PCM_FORMAT_S16_LE
#else
											  SND_PCM_FORMAT_S16_BE
#endif
						  ) < 0))
   {
#ifndef __BIG_ENDIAN__
	   printf(ALSA_PRN "could not set PCM format (signed 16bit little endian)\n");
#else
	   printf(ALSA_PRN "could not set PCM format (signed 16bit big endian)\n");
#endif
	   result = FALSE;
   }

   /* Set sample rate */
   tmp = inst->sample_rate;
   if(result && (snd_pcm_hw_params_set_rate_near(inst->handle,
												 inst->params,
												 &tmp,
												 0) < 0))
   {
	   printf(ALSA_PRN "could not set sample rate[%d] \n", tmp);
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "sample rate set[%d]\n", tmp);
   }


   if(result && (tmp != inst->sample_rate))
   {
	   printf(ALSA_PRN "sample rate is not supported by the hardware\n");
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "actual sample rate matches requested rate\n");
   }



   /* Channels */
   if(result && (snd_pcm_hw_params_set_channels(inst->handle,
												inst->params,
												inst->channels) < 0))
   {
	   printf(ALSA_PRN "could not set number of channels\n");
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "number of channels set[%d]\n", inst->channels);
   }


   /* Periods (buffers/fragments) */
   if(result && (snd_pcm_hw_params_set_periods(inst->handle,
											   inst->params,
											   PERIODS,
											   0) < 0))
   {
	   printf(ALSA_PRN "could not set number of periods\n");
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "periods set\n");
   }


   /* Set buffer size. We alsays use 16 bit (2 byte) mode, but we must also take
	* account for the number of channels */
   inst->framesize = 2 * inst->channels;
   tmp = PERIOD_SIZE * PERIODS / inst->framesize;
   if(result && (snd_pcm_hw_params_set_buffer_size(inst->handle,
												   inst->params,
												   tmp) < 0))
   {
	   printf(ALSA_PRN "could not set buffer size (%i)\n", tmp);
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "buffer size set\n");
   }

   /* Apply parameters */
   if(result && (snd_pcm_hw_params(inst->handle,
								   inst->params) < 0))
   {
	   printf(ALSA_PRN "could not set hw parameters\n");
	   result = FALSE;
   }
   else
   {
	   printf(ALSA_PRN "parameters applied\n");
   }

   return result;
}

/* Close alsa configuration */
static CsrBool alsa_close(void **instance)
{
   alsa_instance_t *inst;
   int code;
   CsrBool result;
   inst = (alsa_instance_t*)*instance;

   /* Close the handle */
   result = TRUE;
   if(inst->handle != 0)
   {
	   code = snd_pcm_close(inst->handle);
	   inst->handle = 0;
	   if(code < 0)
	   {
		   printf(ALSA_PRN "error closing PCM handle, code %i\n", code);
		   result = FALSE;
	   }
   }

   return result;
}
   
/* Start alsa streaming */
static CsrBool alsa_start(void **instance)
{
   alsa_instance_t *inst;
   int code;
   CsrBool result;
   inst = (alsa_instance_t*)*instance;

   /* Prepare the handle */

   printf(ALSA_PRN "preparing pcm\n");

   code = snd_pcm_prepare(inst->handle);
   if(code < 0)
   {
	   printf(ALSA_PRN "error preparing PCM handle, code %i\n", code);
	   result = FALSE;
   }
   else
   {

	   printf(ALSA_PRN "starting alsa play\n");

	   inst->errors = 0;
	   inst->run = TRUE;

#if 1
	   /* Start thread */
	   /* Playback thread */
	   code = pthread_create(&inst->thread,
							 NULL,
							 alsa_thread_play,
							 inst);
	   if(code != 0)
	   {
		   printf(ALSA_PRN "could not start playback thread, code %i\n", code);
		   result = FALSE;
	   }
	   else
	   {
		   result = TRUE;
#ifdef DEBUG
		   printf(ALSA_PRN "playback thread started\n");
#endif

	   }

#endif

   }

   return result;
}
   
/* Stop alsa streaming */
static CsrBool alsa_stop(void **instance)
{
   alsa_instance_t *inst;
   int code;
   void *thres;
   CsrBool result;
   inst = (alsa_instance_t*)*instance;

   /* Stop thread */
   inst->run = FALSE;

#if 1
   if(inst->thread != 0)
   {
	   pthread_cond_broadcast(&inst->wake);
	   code = pthread_join(inst->thread, &thres);
	   if(code != 0)
	   {
		   printf(ALSA_PRN "error stopping thread, code %i\n", code);
	   }
	   inst->thread = 0;
   }
   else
   {
	   printf(ALSA_PRN "thread not running\n");
   }
#endif

   /* Drain the handle */
   code = snd_pcm_drain(inst->handle);
   if(code < 0)
   {
	   printf(ALSA_PRN "error draining (stopping) PCM handle, code %i\n", code);
	   result = FALSE;
   }
   else
   {
	   result = TRUE;
   }

   return result;
}

static CsrBool alsa_play(void **instance, void *data, CsrInt32 datalen)
{
	alsa_instance_t *inst;
//    CsrInt32 frames;
//    CsrInt32 res;
	CsrBool result=TRUE;
	
	inst = (alsa_instance_t*)*instance;
#if 0
	if(inst->run && !(inst->suspend))
	{

        /* Send data to ALSA */
        frames = datalen;
        res = snd_pcm_writei(inst->handle,
                             data,
                             frames);
		result = TRUE;
        if(res < 0)
        {
            alsa_xrun(inst, res);
			result = FALSE;
        }
        else if(res != frames)
        {
            printf(ALSA_PRN "playback device did not consume all frames, sent %i, consumed %i\n",
                   frames, res);
			result = FALSE;
        }
	}
	else
	{
		printf(ALSA_PRN " dose not play run(%s), suspend(%s) %i\n", (inst->run?"true":"false"), (inst->suspend?"true":"false") );
		result = TRUE;
	}
#else

/* Put data into incoming buffer */
	CsrMemCpy(inst->buffers[inst->buf_in] + inst->lengths[inst->buf_in],
		   data,
		   datalen);
	inst->lengths[inst->buf_in] += datalen;

	if(inst->lengths[inst->buf_in] >= (BUFFER_SIZE * BUFFER_THRESHOLD))
	{
		/* Select next buffer and prepare to receive more data */
		inst->buf_in = (inst->buf_in + 1) % BUFFERS;
		inst->lengths[inst->buf_in] = 0;

		/* Update shared couter and wake thread if necessary */
		pthread_mutex_lock(&inst->mutex);
		inst->count++;
		if(inst->count >= PLAYBACK_THRESHOLD)
		{
			pthread_cond_signal(&inst->wake);
		}
		pthread_mutex_unlock(&inst->mutex);
	}

	/* Free data and trigger next stage even though data has been consumed */
//	if(freeData && data)
//	{
//		CsrPmemFree(data);
//	}


#endif
	return result;
}

#endif

   
CsrBtScDbDeviceRecord rec_db[5];
int rec_idx=0;
BD_ADDR_T tst_bdaddr;
   /* GAP_SC_PASSKEY_IND 	*/
void csrBtGapScPassKeyInd(CsrBtDeviceAddr  DeviceAddr, /* address of device for which a passkey is requested                           */
    	CsrBtDeviceName  DeviceName,  /* name of device for which a passkey is requested                              */
    	CsrUint8   Paired,             /* actual pair status of the device                                             */
    	CsrUint32    cod)
{
   CsrUint8         thePasskey[4]={'0','0','0','0'};
   printf("csrBtGapScPassKeyInd name[%s] [%x]\n",DeviceName,cod);

   csr_gap_sc_passkey_res_cmd(TRUE,DeviceAddr,4,thePasskey,FALSE);
}
      /* GAP_SC_SSP_JUSTWORS_IND	*/
void csrBtGapScSspJustworksInd(CsrBtDeviceAddr  DeviceAddr, /* address of device for which a passkey is requested                           */
    	CsrBtDeviceName  DeviceName,  /* name of device for which a passkey is requested                              */
    	CsrUint8   Paired,             /* actual pair status of the device                                             */
    	CsrUint32    cod)
{
   printf("csrBtGapScSspJustworksInd [%s] [%x]\n",DeviceName,cod);
   csr_gap_sc_justworks_res_cmd(TRUE,DeviceAddr,FALSE);
}
	  /* GAP_SC_BOND_IND	*/
void csrBtGapScBondIndFunc(CsrBool				 addedToScDbList,	 /* If TRUE the device is added to the Security Manager's device database		 */
		  CsrBtDeviceAddr		 deviceAddr,		 /* address of device that has been bonded										 */
    	  CsrBtResultCode   resultCode,               /* the result of the outgoing bonding procedure                                 */
		  CsrBtSupplier    resultSupplier,                             
    	  CsrBtScDbDeviceRecord * pRecord
    	)
{
//resultCode:CSR_BT_RESULT_CODE_SC_SUCCESS
//resultSupplier:CSR_BT_SUPPLIER_SC
	printf("csrBtGapScBondIndFunc resultCode[%x]resultSupplier[%x]\n",resultCode,resultSupplier);
	printf("csrBtGapScBondInd BDADDR %04X:%02X:%06X\n",deviceAddr.nap, deviceAddr.uap, deviceAddr.lap);

    if(pRecord)
    {


		printf("csrBtGapScBondInd   authrized %d likkeyType %d\n",pRecord->authorised, pRecord->linkkeyType);
		printf("csrBtGapScBondInd    remoteName [%s] cod    [%x]\n",pRecord->remoteName, pRecord->classOfDevice);
		printf("link key:");
		{
			int i;
			for(i=0;i<SIZE_LINK_KEY; i++)
			  printf("%x ",pRecord->Linkkey[i]);
			}
#if 1	
			CsrMemCpy(&rec_db[rec_idx],pRecord,sizeof(CsrBtScDbDeviceRecord));
		    rec_idx++;

#else
		    CsrMemCpy(&rec_db.deviceAddr,&pRecord->deviceAddr,sizeof(BD_ADDR_T));
			rec_db.linkkeyValid=pRecord->linkkeyValid;
			printf("\n rec_db.linkkeyValid %d ",pRecord->linkkeyValid);
			CsrMemCpy(&rec_db.Linkkey,&pRecord->Linkkey,sizeof(CsrBtDeviceLinkkey));
			CsrMemCpy(&rec_db.remoteName,&pRecord->remoteName,sizeof(CsrBtDeviceName));
			CsrMemCpy(&rec_db.classOfDevice,&pRecord->classOfDevice,sizeof(CsrBtClassOfDevice));
			rec_db.authorised=pRecord->authorised;
			printf("rec_db.authorised %d ",pRecord->authorised);
			rec_db.linkkeyType=pRecord->linkkeyType;
			rec_db.linkkeyLen=pRecord->linkkeyLen;
			printf("rec_db.linkkeyLen %d ",pRecord->linkkeyLen);
			printf("rec_db.linkkeyType %d ",pRecord->linkkeyType);
#endif	
		printf("\n");
    }
	else
	{
	   printf("pRecord is null\n");
	}
}
void csrBtGapScAuthorizeIndFunc(CsrBtDeviceAddr  DeviceAddr, CsrBtDeviceName  DeviceName,  CsrUint16   Service)
{
    printf("csrBtGapScAuthorizeIndFunc:%s\n",DeviceName);
	printf("csrBtGapScAuthorizeIndFunc: service %x\n",Service);
	printf("csrBtGapScAuthorizeIndFunc BDADDR %04X:%02X:%06X\n",DeviceAddr.nap, DeviceAddr.uap, DeviceAddr.lap);
    csr_gap_sc_authorize_rsp_cmd(TRUE,DeviceAddr);
}
    /*GAP_LOCAL_BDADDR_CFM*/
void csrBtGapCmLocalBdaddrCfmFunc(CsrBtDeviceAddr		 deviceAddr)
{   
	printf("csrBtGapCmLocalBdaddrCfmFunc: %04X:%02X:%06X\n", deviceAddr.nap, deviceAddr.uap, deviceAddr.lap);
}
	/*GAP_LOCAL_NAME_CFM*/
void csrBtGapCmLocalNameCfmFunc(CsrBtDeviceName	name)
{
  printf("csrBtGapCmLocalNameCfmFunc:%s\n",name);

}



void csrBtGapSDSearchIndFunc(GAP_SD_SEARCH_IND_T * searchdata)
{
  printf("csrBtGapSDSearchIndFunc:%s \n",searchdata->friendlyName);
  printf("csrBtGapSDSearchIndFunc COD:%x \n",searchdata->deviceClass);
  printf("csrBtGapSDSearchIndFunc %04X:%02X:%06X\n", searchdata->deviceAddr.nap, searchdata->deviceAddr.uap, searchdata->deviceAddr.lap);
  printf("csrBtGapSDSearchIndFunc pid [0x%4x]  vid [0x%4x] rssi [%d]\n",searchdata->pid, searchdata->vid,searchdata->rssi);
  tst_bdaddr=searchdata->deviceAddr;

}


void csrBtGapSDSearchCloseFunc()
{
  printf("csrBtGapSDSearchCloseFunc:\n");

}

void csrBtGapChiWdCountFunc(CsrUint16 varId)
{
   printf("csrBtGapChiWdCountFunc:\n");
}

void csrBtGapGetErrorIndFunc(void)
{
   printf("csrBtGapGetErrorIndFunc:\n");
}
void csrBtGapBtInitIndFunc(void)
{
   printf("csrBtGapBtInitIndFunc:\n");
}
void csrBtGapGetDbgLevelIndFunc(CsrUint32 dbgLevel)
{
  printf("csrBtGapGetDbgLevelIndFunc : %d\n", dbgLevel);
}

void csrBtGapScDeregisterScdbCfmFunc(CsrBtDeviceAddr         deviceAddr,CsrBtResultCode   resultCode,CsrBtSupplier    resultSupplier)
{
   printf("csrBtGapScDeregisterScdbCfmFunc: %04X:%02X:%06X\n", deviceAddr.nap, deviceAddr.uap, deviceAddr.lap);
}
void csrBtScEncryptionCfmFunc(CsrBtDeviceAddr		   deviceAddr,	  CsrBtResultCode		  resultCode,CsrBtSupplier			 resultSupplier)
{
   printf("csrBtScEncryptionCfmFunc: %04X:%02X:%06X\n", deviceAddr.nap, deviceAddr.uap, deviceAddr.lap);
}

static  struct CsrBtAppGapEventHandlerStruct csrBtGapAppEventFunctions =
{
	csrBtGapScPassKeyInd,      		     /* GAP_SC_PASSKEY_IND  */
	csrBtGapScSspJustworksInd,          /* GAP_SC_SSP_JUSTWORS_IND  */
	csrBtGapScBondIndFunc,      	    /*GAP_SC_BOND_IND*/
	csrBtGapScAuthorizeIndFunc,         /*GAP_SC_ATHORIZE_IND*/
	csrBtGapCmLocalBdaddrCfmFunc,       /*GAP_LOCAL_BDADDR_CFM*/
	csrBtGapCmLocalNameCfmFunc,        /*GAP_LOCAL_NAME_CFM*/
	csrBtGapSDSearchIndFunc,           /*GAP_SD_SEARCH_IND*/
	csrBtGapSDSearchCloseFunc,         /*GAP_SD_SEARCH_CLOSE*/                                       
	csrBtGapChiWdCountFunc,             /*GAP_CHIP_WD_COUNT*/
	csrBtGapGetDbgLevelIndFunc,	    	/*GAP_GET_DBG_LEVEL_IND*/
	csrBtGapGetErrorIndFunc,               /*GAP_ERROR_IND*/
	csrBtGapBtInitIndFunc,                 /*GAP_BT_INIT_IND*/
	csrBtGapScDeregisterScdbCfmFunc,        /*GAP_SC_DEREGISGER_SCDB_CFM*/
	csrBtScEncryptionCfmFunc				/*GAP_SC_ENCRYPTION_CFM*/
};


void csrBtSSAVAtivateCfm(void)
{
   printf("csrBtSSAVAtivateCfm\n");
}

void csrBtSSAVDeActivateCfm(void)
{
    printf("csrBtSSAVDeActivateCfm\n");
}
BD_ADDR_T a2dp_addr;

void csrBtSSAVConnectInd(BD_ADDR_T addr)
{
    printf("csrBtSSAVConnectInd %04X:%02X:%06X\n", addr.nap, addr.uap, addr.lap);
 #ifdef ALSA_PLAY  	
	alsa_init((void**)&alsa_inst);
 #endif
    CsrMemCpy(&a2dp_addr,&addr,sizeof(a2dp_addr));
}
void csrBtSSAVConfigInd( CsrUint16 pcm_sampling_rate, CsrUint8	 pcm_no_channels)
{
   printf("csrBtSSAVConfigInd pcm_sampling_rate [%d] pcm_no_channels [%d]\n",pcm_sampling_rate,pcm_no_channels);
 #ifdef ALSA_PLAY  
   setAlsaPcmSettings(pcm_no_channels, DEFAULT_BPS,pcm_sampling_rate);
   alsa_open((void **)&alsa_inst);
   usleep(2);
   alsa_start((void **)&alsa_inst);
 #endif  
}

void csrBtSSAVPcmStreamDataInd(CsrUint8 *p_data, CsrUint16 len, CsrUint32 sampling_rate, CsrUint8 number_of_channelss)
{
  // usleep(10000);
 //   printf("csrBtSSAVPcmStreamDataInd len [%d]\n",len);
#ifdef ALSA_PLAY    
  if(!compareAlsaPcmSettings(number_of_channelss, DEFAULT_BPS, sampling_rate))
  {
     printf("csrBtSSAVPcmStreamDataInd reconfigred PCM \n");
	alsa_stop((void **)&alsa_inst);
	setAlsaPcmSettings(number_of_channelss, DEFAULT_BPS,sampling_rate);
	alsa_open((void **)&alsa_inst);
	usleep(2);
	alsa_start((void **)&alsa_inst);
	
  }

  alsa_play((void **)&alsa_inst, (void *)p_data, (CsrUint32)len);
#endif  
//  CsrPmemFree(p_data);
} 
void csrBtSSAVCloseInd(void)
{
   printf("csrBtSSAVCloseInd\n");
 #ifdef ALSA_PLAY    
   alsa_stop((void **)&alsa_inst);
   alsa_close((void **)&alsa_inst);
 #endif
}

void csrBtSSAVCloseCfm(CsrBool result)
{
   printf("csrBtSSAVCloseCfm\n");
}

void csrBtSSAVConnectCfm(BD_ADDR_T addr,CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier)
{
   printf("csrBtSSAVConnectCfm result[%x] avResultSupplier[%x]\n",avResultCode,avResultSupplier);
 #ifdef ALSA_PLAY   
   alsa_init((void **)&alsa_inst);
 #endif 
    CsrMemCpy(&a2dp_addr,&addr,sizeof(a2dp_addr));
}
void csrBtSSAVDisconnectInd(BD_ADDR_T addr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier)
{
 

   printf("csrBtSSAVDisconnectInd %04X:%02X:%06X\n", addr.nap, addr.uap, addr.lap);
   printf("reasonCode %x,reasonSupplier %x\n",reasonCode, reasonSupplier );
#ifdef ALSA_PLAY 
   if(alsa_inst)
   {
	   if(alsa_inst->run)
	   {
	      alsa_stop((void **)&alsa_inst);
		  usleep(2);
		  alsa_close((void **)&alsa_inst);
	   }
	   alsa_deinit((void **)&alsa_inst);
   }
 //  csr_av_connect_cmd(a2dp_addr);
#endif   
}

void csrBtAvOpenInd(CsrBool  ss_filter)
{
    printf("csrBtAvOpenInd %d\n",ss_filter);
}

void csrBtSSAVConfigCfm( CsrUint16 pcm_sampling_rate, CsrUint8	 pcm_no_channels)
{
   printf("csrBtSSAVConfigCfm pcm_sampling_rate [%d] pcm_no_channels [%d]\n",pcm_sampling_rate,pcm_no_channels);
 #ifdef ALSA_PLAY  
   setAlsaPcmSettings(pcm_no_channels, DEFAULT_BPS,pcm_sampling_rate);
   alsa_open((void **)&alsa_inst);
   usleep(2);
   alsa_start((void **)&alsa_inst);
 #endif  
}
void csrBtAvOpenCfm(CsrBool  ss_filter)
{
    printf("csrBtAvOpenCfm %d\n",ss_filter);
}

void csrBtSSAVDisconnectCfm(BD_ADDR_T addr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier)
{
 

   printf("csrBtSSAVDisconnectCfm %04X:%02X:%06X\n", addr.nap, addr.uap, addr.lap);
   printf("reasonCode %x,reasonSupplier %x\n",reasonCode, reasonSupplier );
#ifdef ALSA_PLAY 
   if(alsa_inst)
   {
	   if(alsa_inst->run)
	   {
	      alsa_stop((void **)&alsa_inst);
		  usleep(2);
		  alsa_close((void **)&alsa_inst);
	   }
	   alsa_deinit((void **)&alsa_inst);
   }
 //  csr_av_connect_cmd(a2dp_addr);
#endif   
}
void csrBtAvStartInd(void)
{
    printf("csrBtAvStartInd\n");
}
void csrBtAvSuspendInd(void)
{
   printf("csrBtAvSuspendInd\n");
}

static  struct CsrBtAppAVEventHandlerStruct csrBtAVAppEventFunctions =
{
	csrBtSSAVAtivateCfm,      		/* AV_ACTIVATATE_CFM            */
	csrBtSSAVDeActivateCfm,   		/* AV_DEACTIVATATE_CFM        */
	csrBtSSAVConnectInd,            /* AV_CONNECT_IND */
	csrBtSSAVConfigInd,            	/* AV_CONFIG_IND           */
	csrBtSSAVPcmStreamDataInd,     	/* AV_PCM_STREAM_DATA_IND       */
	csrBtSSAVCloseInd,            	/* AV_CLOSE_IND          */
	csrBtSSAVCloseCfm, 				/*AV_CLOSE_CFM*/
	csrBtSSAVConnectCfm,			/*AV_CONNECT_CFM*/
	csrBtSSAVDisconnectInd, 		    /*AV_DISCONNECT_IND*/
	csrBtAvOpenInd,                     /*AV_OPEN_IND,*/
	csrBtSSAVConfigCfm,                 /*AV_CONFIG_CFM*/
	csrBtAvOpenCfm,                     /*AV_OPEN_CFM*/
	csrBtSSAVDisconnectCfm,               /*AV_DISCONNECT_CFM,*/
	csrBtAvStartInd,                      /*AV_START_IND*/
	csrBtAvSuspendInd
};


void csrBtAVRCAtivateCfm(void)
{
   printf("csrBtAVRCAtivateCfmFunc\n");
}
void csrBtAVRCDeActivateCfm(void)
{
   printf("csrBtAVRCDeActivateCfmFunc\n");
}
void csrBtAVRCConnectInd(BD_ADDR_T bdAddr)
{
   printf("csrBtAVRCConnectIndFunc %04X:%02X:%06X\n",bdAddr.nap,bdAddr.uap, bdAddr.lap);
}
void csrBtAVRCRemoteFeturesInd(CsrBtAvrcpConfigSrFeatureMask   srFeatures)
{
   printf("csrBtAVRCRemoteFeturesInd %x\n" ,srFeatures);
}

void csrBtAVRCCtPassThroughCfm(CsrBtAvrcpPTOpId operationId, CsrBtResultCode             resultCode,CsrBtSupplier          avResultSupplier)
{
   printf("csrBtAVRCCtPassThroughCfm 0x%x  result: 0x%x\n",operationId, resultCode);    //CSR_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED
}

void csrBtAVRCCloseInd(void)
{
   printf("csrBtAVRCCloseInd\n");
}
void csrBtAVRCConnectCfm(BD_ADDR_T addr,CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier)
{
   printf("csrBtAVRCConnectCfm avResultCode[%x]avResultSupplier[%x]\n",avResultCode,avResultSupplier);
}

void csrBtAVRCDisconnectInd(BD_ADDR_T addr,CsrUint16 reasonCode,CsrUint16 reasonSupplier)
{
    printf("csrBtAVRCDisconnectInd %04X:%02X:%06X\n",addr.nap,addr.uap, addr.lap);
}

void csrBtAVRCCtPassThroughInd(CsrBtAvrcpPTOpId operationId, CsrBtAvrcpPTState             state)
{
   printf("csrBtAVRCCtPassThroughInd 0x%x  state: 0x%x\n",operationId, state);    //CSR_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED
}

void csrBtAVRCDisconnectCfm(BD_ADDR_T addr,CsrUint16 reasonCode,CsrUint16 reasonSupplier)
{
    printf("csrBtAVRCDisconnectCfm %04X:%02X:%06X\n",addr.nap,addr.uap, addr.lap);
}


static  struct CsrBtAppAVRCEventHandlerStruct csrBTAvrcCallbackFunctions=
{
    csrBtAVRCAtivateCfm,   				/*AVRC_ACTIVATATE_CFM    */
    csrBtAVRCDeActivateCfm, 			/*AVRC_DEACTIVATATE_CFM      */
    csrBtAVRCConnectInd,	 			/*AVRC_CONNECT_IND*/
    csrBtAVRCRemoteFeturesInd,	 		/*AVRC_REMOTE_FEATURES_IND*/
    csrBtAVRCCtPassThroughCfm,          /*AVRC_CT_PASS_THROUGH_CFM*/
    csrBtAVRCCloseInd,	  				/*AVRC_CLOSE_IND*/
    csrBtAVRCConnectCfm,				/*AVRC_CONNECT_CFM*/
    csrBtAVRCDisconnectInd,              /* AVRC_DISCONNECT_IND,*/
    csrBtAVRCCtPassThroughInd,            /*AVRC_TG_PASS_THROUGH_IND*/
    csrBtAVRCDisconnectCfm              /* AVRC_DISCONNECT_CFM,*/
};

void csrBtHidhAtivateCfm(void)
{
	printf("csrBtHidhAtivateCfm\n");

}
void csrBtHidhDeActivateCfm(void)
{
	printf("csrBtHidhDeActivateCfm\n");

}
void csrBtHidhConnectInd(CsrBtDeviceAddr bdAddr)
{
	printf("csrBtHidhConnectInd\n");

}
void csrBtHidhConnectCfm(CsrBtResultCode       resultCode,CsrBtSupplier   resultSupplier, CsrAppBtHidhSdpInfoData  * infodata)
{
	printf("csrBtHidhConnectCfm\n");
	if (resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS &&resultSupplier == CSR_BT_SUPPLIER_HIDH)
	{
		if((infodata->sdpInfo !=NULL) &&(infodata->sdpInfo->deviceSubclass % CSR_BT_PERIPHERAL_MAJOR_DEVICE_MASK))
		{
		   printf("%x %x %x %x\n",infodata->sdpInfo->isValid,infodata->sdpInfo->flags,infodata->sdpInfo->vendorId,infodata->sdpInfo->productId);
		   printf("%x %x %x %x\n",infodata->sdpInfo->deviceVersion,infodata->sdpInfo->deviceSubclass,infodata->sdpInfo->countryCode,infodata->sdpInfo->profileVersion);

           if(infodata->sdpInfo)
           	{
           	   CsrMemCpy(&mysdpInfo,infodata->sdpInfo,sizeof(CsrBtHidhSdpInfo));
			   myinfodata.sdpInfo=&mysdpInfo;
           	}
		  if(infodata->serviceName)
		  {
		     CsrStrCpy(myserviceName,infodata->serviceName);
			 myinfodata.serviceName=myserviceName;
		  }
		   if(infodata->descriptorLength)
		   {
		   	   CsrMemCpy(mydescriptor,infodata->descriptor,infodata->descriptorLength);
			   myinfodata.descriptor=mydescriptor;
			   myinfodata.descriptorLength=infodata->descriptorLength;
		   }
		   
		   csr_gap_sc_encryption_req_cmd(TRUE,infodata->deviceAddr);
		}

	}
	else
	{
	   	printf("csrBtHidhConnect error\n");
	}
}
void csrBtHidhDisconnectInd (CsrBtDeviceAddr bdAddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier)
{
	printf("csrBtHidhDisconnectInd\n");

}

void csrBtHidhDataInd(CsrUint16 			 dataLen, CsrUint8				 *data)
{
	 CsrUint16 i, startDataIndex = 0;

    if (1 == data[1])
    {
        printf("Keyboard(%02x): ", data[1]);
        startDataIndex = 2;
    }
    else if (2 == data[1])
    {
        printf("Mouse(%02x): buttons(%02x), X(%i), Y(%i) ", data[1], data[2], (CsrInt8)data[3], (CsrInt8)data[4]);
        startDataIndex = 5;
    }
    for(i=startDataIndex; i<dataLen;i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n%d bytes received\n", dataLen);
}
void csrBtHidhStatusInd(CsrBtHidhStatus		   status)
{
    printf("csrBtHidhStatusInd %d\n", status);
}
void csrBtHidhConnectAcceptInd(BD_ADDR_T addr,CsrBtResultCode 	   avResultCode, CsrBtSupplier			avResultSupplier)
{
	printf("csrBtHidhConnectAcceptInd\n");

}

void csrBtHidhDisconnectCfm (CsrBtDeviceAddr bdAddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier)
{
   printf("csrBtHidhDisconnectCfm\n");
}


static  struct CsrBtAppHIDHEventHandlerStruct csrBTHidhCallbackFunctions=
{
    csrBtHidhAtivateCfm,   				/*HIDH_ACTIVATATE_CFM    */
    csrBtHidhDeActivateCfm, 			/*HIDH_DEACTIVATATE_CFM      */
    csrBtHidhConnectInd,	 			/*HIDH_CONNECT_IND*/
    csrBtHidhConnectCfm,	 		    /*HIDH_CONNECT_CFM*/
    csrBtHidhDisconnectInd,             /*HIDH_DISCONNECT_IND*/
    csrBtHidhDataInd,                   /*HIDH_DATA_IND*/
    csrBtHidhStatusInd,                 /*HIDH_STATUS_IND*/
    csrBtHidhConnectAcceptInd,           /*HIDH_CONNECT_ACCEPT_CFM*/
    csrBtHidhDisconnectCfm               /*HIDH_DISCONNECT_CFM*/
};


void app_display_menu(void)
{
   printf("1) AV_ACTIVATE/AVRCACTIVATE/enable Scan/enable Connect\n");
   printf("2) AV_DEACTIVATE/AVRCDEACTIVATE\n");
   printf("3) disconect\n");
   printf("4) search\n");
   printf("5) play\n");
   printf("6) pause\n");
   printf("7) fforward Connect\n");
   printf("8) Deregister\n");
   printf("9) register\n");
   printf("10) csr_gap_init_wd_count\n");
   printf("11) csr_gap_get_wd_count\n");
   printf("12) csr_dev_info\n");
   printf("13) set log\n");
   printf("14) clear log\n");
   printf("15) AV/AVRCP connect\n");
   printf("16) AV/AVRCP disconnect\n");
   printf("17) csr_hidh_connect_cmd\n");
   printf("18) csr_hidh_disconnect_cmd\n");
   printf("19) csr_hidh_connect_accecpt_cmd\n");
   printf("19) csr_hidh_data_cmd\n");
   printf("\n");
   
}
int intput_key()
{
   int key;
   int result=-1;
   char buf[300];

   scanf("%300s",buf);
   
   if(sscanf(buf, "%300d", &key) != 1)
   {
      result=-1;
   }
   else
   	{
   		result = key;
   	}
   
   return result; 
}
void csrBtMgtServerProcessClose(void)
{
   printf("closed  processs\n");
}


void app_menu()
{
    BD_ADDR_T bdaddr;
	CsrBtDeviceName            localName="[Samsung] MY";
   do
   {
     app_display_menu();
	
	 switch(intput_key())
	 {
	    case 0:
//			csr_client_mgt_connect();  //for test
			break;

	    case 1:
			csr_av_active_cmd(&csrBtAVAppEventFunctions);
			csr_avrc_active_cmd(&csrBTAvrcCallbackFunctions);
			csr_hidh_active_cmd(&csrBTHidhCallbackFunctions);
			csr_gap_set_scan_enable_cmd(FALSE,FALSE);	//enable scan
			csr_gap_gap_set_local_name_cmd(localName);
			break;
		case 2:
			csr_av_deactvie_cmd();
			csr_avrc_deactive_cmd();
			csr_hidh_deactive_cmd();
			break;	
		case 3:
			//csr_avrc_ct_pass_through_cmd(CSR_BT_AVRCP_PT_OP_ID_PLAY,CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
			csr_av_disconnect_cmd(bdaddr);
			csr_avrc_disconnect_cmd(bdaddr);
			break;
		case 4:
			csr_gap_sd_search_cmd(5,7000,1200,0x00, FALSE);
		//	csr_avrc_ct_pass_through_cmd(CSR_BT_AVRCP_PT_OP_ID_STOP,CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
			break;
		case 5:
	//		 csr_gap_set_scan_enable_cmd(FALSE,TRUE);	//enable scan
			csr_avrc_ct_pass_through_cmd(CSR_BT_AVRCP_PT_OP_ID_PLAY,CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
			break;
		case 6:	
			csr_avrc_ct_pass_through_cmd(CSR_BT_AVRCP_PT_OP_ID_PAUSE,CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
			break;
		case 7:	
			csr_avrc_ct_pass_through_cmd(CSR_BT_AVRCP_PT_OP_ID_FFORWARD,CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
			break;	
		case 8:
			{
				int i=0;
				for(i=0; i<rec_idx;  i++)
					csr_gap_sc_deregister_scdb_cmd(rec_db[i].deviceAddr);
			}
			break;
		case 9:
			{
				int i=0;
				for(i=0; i<rec_idx;  i++)
					csr_gap_sc_register_scdb_cmd(&rec_db[i]);
			}
			break;
#ifdef __CSR_COUNT_WD__			
		case 10:
			csr_gap_init_wd_count();
			break;
		case 11:
		{
			unsigned short wd_cnt = 0;
			wd_cnt = csr_gap_get_wd_count();
			printf("CSR Client test : watch dog count<%d>\n", wd_cnt);
			break;
		}
#endif	
         case 12:
		 {
		 	BluetoothCsrDeviceInfo  DeviceInfo;
		 	csr_gap_get_local_device_info(&DeviceInfo);
			printf("csrBtAVRCAtivateCfmFunc\n");
			printf("name %s\n",DeviceInfo.friendlyName);
			printf("connectable %d\n",DeviceInfo.connectable);
			printf("discoverable %d\n",DeviceInfo.discoverable);
			break;
         }
		 case 13:
			csr_gap_set_dbg_level(0x05);
		    break;

		case 14:
			csr_gap_set_dbg_level(0x00);
			break;
		case 15:
			{
				//D017:6A:7DDB3B
				a2dp_addr.nap=0xD017;//0xD017;//;
				a2dp_addr.uap=0x6A;//0x6A;//;
				a2dp_addr.lap=0x7DDB3B;//0x7DDB3B;//;		
			csr_av_connect_cmd(a2dp_addr);
			csr_avrc_connect_cmd(a2dp_addr);
			}
			break;
		case 16:
			{
				a2dp_addr.nap=0x1234;//0xD017;//;
				a2dp_addr.uap=0x56;//0x6A;//;
				a2dp_addr.lap=0x789012;//0x7DDB3B;//;
			csr_av_disconnect_cmd(a2dp_addr);
			csr_avrc_disconnect_cmd(a2dp_addr);
			}
			break;
		case 17:
			{
				myinfodata.deviceAddr.nap=0xE811;
				myinfodata.deviceAddr.uap=0x32;
				myinfodata.deviceAddr.lap=0xA40002;
				csr_hidh_connect_cmd(&myinfodata);
			}
			break;
		case 18:
			{
				BD_ADDR_T hidbdaddr;
				hidbdaddr.nap=0xE811;
				hidbdaddr.uap=0x32;
				hidbdaddr.lap=0xA40002;
				//csr_hidh_control_cmd(hidbdaddr,CSR_BT_HIDH_VC_UNPLUG);
				csr_hidh_disconnect_cmd(hidbdaddr);
				break;
			}
		case 19:
			{
				myinfodata.deviceAddr.nap=0xE811;
				myinfodata.deviceAddr.uap=0x32;
				myinfodata.deviceAddr.lap=0xA40002;
				csr_hidh_connect_accecpt_cmd(&myinfodata);
				break;
			}
		case 20:
			{

			    CsrUint16    reportLen=3;
			    CsrUint8     reportData[3];

			    reportData[0] = 0; /* Reserved */
			    reportData[1] = 1;  /*report Id*/
				
				/*1:set activate Num Lock report   
				2: set activate Caps Lock report
				4:set activate scroll Lock report
				0:set deactivate all LEDs report*/
			    reportData[2] = 1; 


			    csr_hidh_data_cmd( CSR_BT_HIDH_OUTPUT_REPORT,
			                    reportLen, reportData);

				break;
			}
	 }
   }while(1);
}

int main(int argc, char **argv)
{
   csr_client_mgt_open(csrBtMgtServerProcessClose,&csrBtGapAppEventFunctions);

   app_menu();

   csr_client_mgt_close();

  return 1;
}
