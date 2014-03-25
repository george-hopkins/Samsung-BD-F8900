/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_platform.h"
#include "Bluetooth_AV.h"
#include "csr_bt_av2_filter.h"
#include "csr_bt_av2_util.h"
#include "Bluetooth_Common.h"
#include "csr_samsung_log.h"

/* Settings */
#define SS_PRN                      "filter_ss: "
#define SS_BURST                    0  /* Split up into this many packets, 0 to disable */

extern Av2AppInst *Av2Instance;


#define DEBUG          1
#define DEBUG_VERBOSE  0


/* SS codec definitions */

#define SS_MEDIA_CODEC_SC_SIZE      11

#ifndef SS_VENDOR_ID
#define SS_VENDOR_ID                       			(0x75)
#endif /* SS_VENDOR_ID */

#define SS_CODEC_ID                   				0x01


/*
*   - bit 6 : 32kHz sampling frequency supported
*   - bit 5 : 44.1kHz sampling frequency supported
*   - bit 4 : 48kHz sampling frequency supported
*   - bit 3 : Mono
*   - bit 1 : Stereo
*/
#define SS_CODEC_SUPPORT_FEATUE                    	0x7A


/* Instance data for this filter */
typedef struct
{
 //   CsrUint8          * name;
    CsrUint16        channel_mode;
    CsrUint16         sample_freq;

} ss_instance_t;

/* Filter entry prototypes */
static CsrBool ss_init(void **instance, av2filter_t *filter, void *unused);
static CsrBool ss_deinit(void **instance);
static CsrBool ss_open(void **instance);
static CsrBool ss_close(void **instance);
static CsrBool ss_start(void **instance);
static CsrBool ss_stop(void **instance);
static CsrBool ss_process_encode(CsrUint8 index,
                                 av2filter_t **chain,
                                 CsrBool freeData,
                                 void *data,
                                 CsrUint32 length);
static CsrBool ss_process_decode(CsrUint8 index,
                                 av2filter_t **chain,
                                 CsrBool freeData,
                                 void *data,
                                 CsrUint32 length);
static CsrUint8 *ss_get_config(void **instance,
                               CsrUint8 *conf_len);
static CsrBtAvResult ss_set_config(void **instance,
                                  CsrUint8 *conf,
                                  CsrUint8 conf_len);
static CsrUint8 *ss_get_caps(void **instance,
                             CsrUint8 *cap_len);
static CsrUint8 *ss_remote_caps(void **instance,
                                CsrUint8 *caps,
                                CsrUint8 caps_len,
                                CsrUint8 *conf_len);
static void ss_qos_update(void **instance,
                           CsrUint16 qosLevel);
static void ss_menu(void **instance,
                     CsrUint8 *num_options);
static CsrBool ss_setup(void **instance,
                        CsrUint8 index,
                        char *value);
static char* ss_return_filename(void **instance);



/* Filter structure setup for SS decoder */
av2filter_t filter_ss_decode =
{
    "csr_ss_decoder",
    FILTER_DECODER | FILTER_PASS,
    CSR_BT_AV_AUDIO,
    CSR_BT_AV_NON_A2DP_CODEC,
    0,
    NULL,
    ss_init,
    ss_deinit,
    ss_open,
    ss_close,
    ss_start,
    ss_stop,
    ss_process_decode,
    ss_get_config,
    ss_set_config,
    ss_get_caps,
    ss_remote_caps,
    ss_qos_update,
    ss_menu,
    ss_setup
};


/* Initialise SS filter */
static CsrBool ss_init(void **instance,
    struct av2filter_t *filter,
    void *unused)
{
	ss_instance_t *inst;
	*instance = CsrPmemZalloc(sizeof(ss_instance_t));
	inst = (ss_instance_t*)*instance;
    /* Set default filename */
   // inst->name = (CsrUint8 *)CsrStrDup("SS_CODEC");


//	Av2Instance->ss_filter_support=FALSE;

	return TRUE;
}


/* Deinitialise SS filter */
static CsrBool ss_deinit(void **instance)
{
    ss_instance_t *inst;
    inst = (ss_instance_t*)*instance;

//	Av2Instance->ss_filter_support=FALSE;

    /* Free instance */
    //CsrPmemFree(inst->name);
    CsrPmemFree(*instance);
    *instance = NULL;

    return TRUE;
}

/* Open SS with current configuration */
static CsrBool ss_open(void **instance)
{


        return TRUE;

}

/* Close SS configuration */
static CsrBool ss_close(void **instance)
{
 //   ss_instance_t *inst;
 //   inst = (ss_instance_t*)*instance;



    /* SS does not require any special close handling */
    return TRUE;
}

/* Start SS streaming */
static CsrBool ss_start(void **instance)
{
//    ss_instance_t *inst;
//    inst = (ss_instance_t*)*instance;


    return TRUE;
}

/* Stop SS streaming */
static CsrBool ss_stop(void **instance)
{
//   ss_instance_t *inst;
//    inst = (ss_instance_t*)*instance;



    /* SS does not require any special stop handling */
    return TRUE;
}




/* Process SS data: Decoder */
static CsrBool ss_process_decode(CsrUint8 index,
                                 av2filter_t **chain,
                                 CsrBool freeData,
                                 void *data,
                                 CsrUint32 length)
{



	
    return chain[index+1]->f_process((CsrUint8)(index+1),
                                     chain,
                                     freeData,       //change false   /// not free 
                                     data,
                                     length);
}


/* Return SS configuration */
static CsrUint8 *ss_get_config(void **instance,
                               CsrUint8 *conf_len)
{
//    CsrUint8 tmp;
    CsrUint8 *conf;
    ss_instance_t *inst;
 //   inst = (ss_instance_t*)*instance;

#ifdef DEBUG
    CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,SS_PRN  "get_config\n"));
#endif

    *conf_len = SS_MEDIA_CODEC_SC_SIZE;
    conf = CsrPmemAlloc(*conf_len);

	conf[0] = CSR_BT_AV_SC_MEDIA_CODEC;
	conf[1] = SS_MEDIA_CODEC_SC_SIZE-2 ;
	conf[2] = CSR_BT_AV_AUDIO<<4;
	conf[3] = CSR_BT_AV_NON_A2DP_CODEC;
	conf[4] = SS_VENDOR_ID;  /*vendor ID*/
    conf[5] = 0x00; 
    conf[6] = 0x00; 
    conf[7] = 0x00;   
    conf[8] = SS_CODEC_ID;  /*codec*/
    conf[9] = 0x00;  
    conf[10]= SS_CODEC_SUPPORT_FEATUE;

    return conf;
}

/* Set SS configuration */
static CsrBtAvResult ss_set_config(void **instance,
                                  CsrUint8 *conf,
                                  CsrUint8 conf_len)
{
//    ss_instance_t *inst;
//    inst = (ss_instance_t*)*instance;

#ifdef DEBUG
    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,SS_PRN  "set_config\n"));
#endif

    /* Dummy check that service capabilities are SS */
    if(!(*(conf+2)>>4 == CSR_BT_AV_AUDIO) ||
       !(*(conf+3) == CSR_BT_AV_NON_A2DP_CODEC) ||!(*(conf+4) == SS_VENDOR_ID))
    {
		CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,SS_PRN "ss_set_config CSR_BT_RESULT_CODE_A2DP_BAD_MEDIA_TRANSPORT_FORMAT\n"));
		return CSR_BT_RESULT_CODE_A2DP_BAD_MEDIA_TRANSPORT_FORMAT;
    }

	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,SS_PRN "ss_set_config CSR_BT_AV_ACCEPT\n"));
	return CSR_BT_AV_ACCEPT;
 
}

/* Return SS service capabilities */
static CsrUint8 *ss_get_caps(void **instance,
                             CsrUint8 *cap_len)
{
	CsrUint8 *cap;
//	 CsrUint8 i;
//	 ss_instance_t *inst;
//	 inst = (ss_instance_t*)*instance;
	 *cap_len = SS_MEDIA_CODEC_SC_SIZE + 2 ;
	 cap = CsrPmemAlloc(*cap_len);
	
	 cap[0] = CSR_BT_AV_SC_MEDIA_TRANSPORT;
	 cap[1] = 0;
	 cap[2] = CSR_BT_AV_SC_MEDIA_CODEC;
	 cap[3] = SS_MEDIA_CODEC_SC_SIZE - 2;
	 cap[4] = CSR_BT_AV_AUDIO<<4;
	 cap[5] = CSR_BT_AV_NON_A2DP_CODEC;
	 cap[6] = SS_VENDOR_ID;    				//vendor ID
	 cap[7] = 0x00; 
	 cap[8] = 0x00; 
	 cap[9] = 0x00;	  
	 cap[10]= SS_CODEC_ID;           		//codec
	 cap[11]= 0x00;	 
	 cap[12]= SS_CODEC_SUPPORT_FEATUE;
	
	 return cap;

}

/* Investigate remote capabilities, return optimal configuration */
static CsrUint8 *ss_remote_caps(void **instance,
                                CsrUint8 *caps,
                                CsrUint8 caps_len,
                                CsrUint8 *conf_len)
{
 //   CsrUint8 i;
    CsrUint8 *conf;
//    ss_instance_t *inst;
 //   inst = (ss_instance_t*)*instance;

    /* Dummy check that service capabilities are SS */
    if(!(*(caps+2)>>4 == CSR_BT_AV_AUDIO) ||
       !(*(caps+3) == CSR_BT_AV_NON_A2DP_CODEC)  ||!(*(caps+4) == SS_VENDOR_ID))
    {
        *conf_len = 0;
        return NULL;
    }

    /* Alloc config */
    *conf_len = SS_MEDIA_CODEC_SC_SIZE;
    conf = (CsrUint8*)CsrPmemAlloc(*conf_len);

    /* Common codec service capability */
    conf[0] = CSR_BT_AV_SC_MEDIA_CODEC;
    conf[1] = SS_MEDIA_CODEC_SC_SIZE - 2;
    conf[2] = CSR_BT_AV_AUDIO<<4;
    conf[3] = CSR_BT_AV_NON_A2DP_CODEC;
	conf[4] = SS_VENDOR_ID; 			/*vendor ID*/
	conf[5] = 0x00; 
	conf[6] = 0x00; 
	conf[7] = 0x00;	 
	conf[8] = *(caps+8); 				/*codec*/
	conf[9] = 0x00;
	conf[10]= *(caps+10);               /*support feature*/

    return conf;
}

/* Print menu options to screen */
static void ss_menu(void **instance,
                     CsrUint8 *num_options)
{
	ss_instance_t *inst;
	char *s;
	inst = (ss_instance_t*)*instance;
#if 0
	s = ((inst->name != NULL)
		 ? inst->name
		 : "n/a");

//	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,SS_PRN "  0) Change filename (%s)\n", inst->name));
#endif

	*num_options = 1;
}


/* Remote sink side buffer level update */
static void ss_qos_update(void **instance,
                           CsrUint16 qosLevel)
{
 //   ss_instance_t *inst;
 //   inst = (ss_instance_t*)*instance;
}

/* Set user option */
static CsrBool ss_setup(void **instance,
                        CsrUint8 index,
                        char *value)
{
	ss_instance_t *inst;
	int i;
	inst = (ss_instance_t*)*instance;
#if 0
	switch(index)
	{
		case 0:
			{
				if(inst->name)
				{
					CsrPmemFree(inst->name);
				}
				inst->name = value;
				value = NULL;
				i = 1;
			}
			break;

		default:
			i = 0;
			break;
	}
#endif
	if(value != NULL)
	{
		CsrPmemFree(value);
	}
	if(i == 0)
	{
		CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,SS_PRN "Wrong selection or invalid format typed.\n"));

	}

	return (i > 0);
}


static char* ss_return_filename(void **instance)
{

    return NULL;
}

