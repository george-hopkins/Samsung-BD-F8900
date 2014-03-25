/******************************************************************************************************

                Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
                    
                      All rights reserved.

FILE:           filter_avrouter.c

DESCRIPTION:    AV2 demo "filter" for interfacing to the AV-router.

REVISION:       $Revision: #6 $

*******************************************************************************************************/
#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_sbc_api.h"
#include "csr_bt_platform.h"
#include "csr_bt_av2.h"
#include "csr_bt_av2_filter.h"
#include "csr_app_lib.h"
#include "csr_bt_av2_dspm.h"
#include "csr_bt_file.h"

/* Settings */
#define AVROUTER_PRN                    "avrouter: "
#define DEFAULT_MAX_BITPOOL          64
#define DEFAULT_MIN_BITPOOL          24
#define SBC_MEDIA_PAYLOAD_TYPE       96
/* #define DEBUG */

/* SBC codec definitions */
#define SC_HDR_SIZE                  2
#define SC_LENGTH_INDEX              1
#define SBC_MEDIA_CODEC_SC_SIZE      8
#define SBC_IE_SAMPLE_FREQ_MASK      ((CsrUint8)0xF0)
#define SBC_IE_CHANNEL_MODE_MASK     ((CsrUint8)0x0F)
#define SBC_IE_BLOCK_MASK            ((CsrUint8)0xF0)
#define SBC_IE_SUBBAND_MASK          ((CsrUint8)0x0C)
#define SBC_IE_ALLOC_METHOD_MASK     ((CsrUint8)0x03)

/* APT-X codec definition */
#define CSR_APTX_MEDIA_CODEC_SC_SIZE     0x0B
#define CSR_APTX_VENDOR_ID               (0x0000004F)
#define CSR_APTX_CSR_VENDOR_ID           (0x0000000A)
#define CSR_APTX_CODEC_ID                (0x0001)
#define CSR_APTX_CODEC_ID2               (0x0002)
#define CSR_APTX_FEATURE_NOT_USED        0xFF

#ifndef CSR_USE_DSPM
/* Instance data for avrouter filter */
typedef struct
{
    CsrUint8          sbc_capabilities[4];
    CsrSbcChannelMode channel_mode;
    CsrSbcAllocMethod alloc_method;
    void             *sbcHdl;
    CsrUint8          channels;
    CsrUint16         sample_freq;
    CsrUint8          blocks;
    CsrUint8          subbands;
    CsrUint8          bitPool;
    CsrUint8          minBitpool;
    CsrUint8          maxBitpool;
    CsrUint8          remoteMinBitpool;
    CsrUint8          remoteMaxBitpool;
    CsrUint16         framesize;
    av2filter_t      *filter;
    av2instance_t    *avInstance;
    char             *filename;
    FILE             *file;
} avrouter_instance_t;
#endif

/* Filter entry prototypes */
static CsrBool avrouter_init(void **instance, 
                             struct av2filter_t *filter,
                             struct av2instance_t *avInst);
static CsrBool avrouter_deinit(void **instance);
static CsrBool avrouter_open(void **instance);
static CsrBool avrouter_close(void **instance);
static CsrBool avrouter_start(void **instance);
static CsrBool avrouter_stop(void **instance);
static CsrBool avrouter_process(CsrUint8 index,
                            av2filter_t **chain,
                            CsrBool freeData,
                            void *data,
                            CsrUint32 length);
static CsrUint8 *avrouter_get_config(void **instance,
                                 CsrUint8 *conf_len);
static CsrBtAvResult avrouter_set_config(void **instance,
                                    CsrUint8 *conf,
                                    CsrUint8 conf_len);
static CsrUint8 *avrouter_get_caps(void **instance,
                               CsrUint8 *cap_len);
static CsrUint8 *avrouter_remote_caps(void **instance,
                                  CsrUint8 *caps,
                                  CsrUint8 caps_len,
                                  CsrUint8 *conf_len);
static void avrouter_qos_update(void **instance,
                             CsrUint16 qosLevel);
static void avrouter_menu(void **instance,
                       CsrUint8 *num_options);
static CsrBool avrouter_setup(void **instance,
                          CsrUint8 index,
                          char *value);
static char* avrouter_return_filename(void **instance);

/* Filter structure setup for avrouter encoder */
struct av2filter_t filter_avrouter_encode =
{
    "csr_avrouter_encoder_control",
    FILTER_ENCODER | FILTER_PASS | FILTER_PRODUCER,
    CSR_BT_AV_AUDIO,
    CSR_BT_AV_SBC,
    0,
    NULL,
    avrouter_init,
    avrouter_deinit,
    avrouter_open,
    avrouter_close,
    avrouter_start,
    avrouter_stop,
    avrouter_process,
    avrouter_get_config,
    avrouter_set_config,
    avrouter_get_caps,
    avrouter_remote_caps,
    avrouter_qos_update,
    avrouter_menu,
    avrouter_setup,
    avrouter_return_filename
};

/* Filter structure setup for avrouter decoder */
struct av2filter_t filter_avrouter_decode =
{
    "csr_avrouter_decoder_control",
    FILTER_DECODER | FILTER_PASS | FILTER_CONSUMER,
    CSR_BT_AV_AUDIO,
    CSR_BT_AV_SBC,
    0,
    NULL,
    avrouter_init,
    avrouter_deinit,
    avrouter_open,
    avrouter_close,
    avrouter_start,
    avrouter_stop,
    avrouter_process,
    avrouter_get_config,
    avrouter_set_config,
    avrouter_get_caps,
    avrouter_remote_caps,
    avrouter_qos_update,
    avrouter_menu,
    avrouter_setup,
    avrouter_return_filename
};

/* Local function to calculate settings given APTX specific codec config */
static CsrUint8 avrouter_store_aptx_config(avrouter_instance_t *inst, CsrUint8 *config)
{
    CsrUint8 tmp;
    CsrUint8 returnValue = 0;

    /* Sample frequency */
    tmp = (CsrUint8)((*config) & SBC_IE_SAMPLE_FREQ_MASK);
    
    if ( tmp & 0x80)
    {
        inst->sample_freq = 16000;
        returnValue = 0x80;
    }
    else if ( tmp & 0x40)
    {
        inst->sample_freq = 32000;
        returnValue = 0x40;
    }
    else if ( tmp & 0x20)
    {
        inst->sample_freq = 44100;
        returnValue = 0x20;
    }
    else
    {
        inst->sample_freq = 48000;
        returnValue = 0x10;
    }

    /* Channel mode */
    tmp = (CsrUint8) ((*config) & SBC_IE_CHANNEL_MODE_MASK);
    if (tmp == 0x08)
    {
        inst->channel_mode = CSR_SBC_MONO;
    }
    else
    {
        inst->channel_mode = CSR_SBC_STEREO;
        returnValue |= CSR_SBC_STEREO;
    }
    
    inst->channels = 1;
    if(inst->channel_mode != CSR_SBC_MONO)
    {
        inst->channels = 2;
    }

    inst->blocks       = 0;
    inst->subbands     = 0;
    inst->alloc_method = 0;

    inst->framesize    = inst->avInstance->maxFrameSize; 
    /* Store new PCM settings */
    setPcmSettings(inst->channels, 16, inst->sample_freq);
    
    return returnValue;
}

/* HCI/RFC platform variable, set in common/hci/rfc_main.c */
extern CsrUint16 targetBitrate;
/* Local function to calculate settings given SBC specific codec config */
static void avrouter_store_config(avrouter_instance_t *inst, CsrUint8 *config)
{
    CsrUint8 i;
    CsrUint8 tmp;
    CsrUint8 mask;

    /* Sample frequency */
    tmp = (CsrUint8)((*config) & SBC_IE_SAMPLE_FREQ_MASK);
    switch( tmp )
    {
        case 0x80:
            inst->sample_freq = 16000;
            break;

        case 0x40:
            inst->sample_freq = 32000;
            break;

        case 0x20:
            inst->sample_freq = 44100;
            break;

        default:
            inst->sample_freq = 48000;
            break;
    }

    /* Channel mode */
    tmp = (CsrUint8) ((*config) & SBC_IE_CHANNEL_MODE_MASK);
    switch (tmp)
    {
        case 0x01:
            inst->channel_mode = CSR_SBC_JOINT_STEREO;
            break;

        case 0x02:
            inst->channel_mode = CSR_SBC_STEREO;
            break;

        case 0x04:
            inst->channel_mode = CSR_SBC_DUAL;
            break;

        case 0x08:
            inst->channel_mode = CSR_SBC_MONO;
            break;
    }
    inst->channels = 1;
    if(inst->channel_mode != CSR_SBC_MONO)
    {
        inst->channels = 2;
    }

    /* Block length */
    config++;
    tmp = (CsrUint8) ((*config) & SBC_IE_BLOCK_MASK);
    mask = 0x80;
    for(i=0; i<4; i++)
    {
        if(tmp == mask )
        {
            inst->blocks = (CsrUint8) ((1+i)<<2);
            break;
        }
        mask >>= 1;
    }

    /* Subbands */
    inst->subbands = (CsrUint8) (12 - ((*config) & SBC_IE_SUBBAND_MASK));

    /* Allocation method */
    inst->alloc_method = CSR_SBC_METHOD_SNR;
    if( ((*config) & SBC_IE_ALLOC_METHOD_MASK) == 1)
    {
        inst->alloc_method = CSR_SBC_METHOD_LOUDNESS;
    }

    /* Min and max bitpools */
    config++;
    inst->minBitpool = CSRMAX(inst->remoteMinBitpool, *config);
    config++;
    inst->maxBitpool = CSRMIN(inst->remoteMaxBitpool, *config);

    /* Calculate new bitpool */
    inst->bitPool = CsrSbcCalcBitPool(&tmp,
                                    &tmp,
                                    inst->channel_mode,
                                    inst->sample_freq,
                                    inst->blocks,
                                    inst->subbands,
                                    targetBitrate);

    if(inst->bitPool > inst->maxBitpool)
    {
        inst->bitPool = inst->maxBitpool;
    }

    inst->framesize = CsrSbcConfig(inst->sbcHdl,
                                 inst->channel_mode,
                                 inst->alloc_method,
                                 inst->sample_freq,
                                 inst->blocks,
                                 inst->subbands,
                                 inst->bitPool);

    /* Store new PCM settings */
    setPcmSettings(inst->channels, 16, inst->sample_freq);
}

/* Initialise avrouter filter */
static CsrBool avrouter_init(void **instance, struct av2filter_t *filter, struct av2instance_t *avInst)
{
    avrouter_instance_t *inst;
    CsrUint8 bits;
    *instance = CsrPmemAlloc(sizeof(avrouter_instance_t));
    inst = (avrouter_instance_t*)*instance;
    CsrMemSet(inst, 0, sizeof(avrouter_instance_t));

    inst->filter = filter;
    inst->avInstance = avInst;

    /* These are just defaults - can be changed online */
    getPcmSettings(&(inst->channels), &bits, &(inst->sample_freq));
    
    inst->sbcHdl = CsrSbcOpen();
    if(inst->channels > 1)
    {
        inst->channel_mode = CSR_SBC_STEREO;
    }
    else
    {
        inst->channel_mode = CSR_SBC_MONO;
    }
    inst->blocks = CSR_SBC_MAX_BLOCKS;
    inst->subbands = CSR_SBC_MAX_SUBBANDS;
    inst->alloc_method = CSR_SBC_METHOD_SNR;
    inst->maxBitpool = DEFAULT_MAX_BITPOOL;
    inst->minBitpool = DEFAULT_MIN_BITPOOL;

    /* Support everything except 48KHz */
    inst->sbc_capabilities[0] = 0xEF;
    inst->sbc_capabilities[1] = 0xFF;
    inst->sbc_capabilities[2] = 0x02;
    inst->sbc_capabilities[3] = 0xFA;


    if (inst->filter->f_type & FILTER_CONSUMER)
    {
        inst->filename = CsrStrDup(CSR_BT_AV_SBC_DEFAULT_FILE_OUT);
    }
    return TRUE;
}

/* Deinitialise avrouter filter */
static CsrBool avrouter_deinit(void **instance)
{
    avrouter_instance_t *inst;
    inst = (avrouter_instance_t*)*instance;

    CsrSbcClose(&inst->sbcHdl);
    CsrPmemFree(inst);
    *instance = NULL;

    return TRUE;
}

/* Open avrouter with current configuration */
static CsrBool avrouter_open(void **instance)
{
    avrouter_instance_t *inst   = (avrouter_instance_t*)*instance;
    CsrBool              result = FALSE;
    
    if ((inst->filename != NULL) && (inst->filter->f_type & FILTER_CONSUMER))
    {
        inst->file = CsrBtFopen(inst->filename, "wb");
        if(inst->file != NULL)
        {
            result = TRUE;
        }
        else
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE, "Could not open file '%s'\n", inst->filename);
        }
    }
    else if (inst->filter->f_type & FILTER_PRODUCER)
    {
        result = TRUE;
    }

    /* Reset timer etc. */
    if(inst->framesize == 0)
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE, "warning: unknown SBC framesize\n");
        inst->framesize = CSR_SBC_MAX_FRAME_SIZE_BYTES;
    }
    
    setMediaPayloadType(SBC_MEDIA_PAYLOAD_TYPE);

    /* Store new PCM settings */
    setPcmSettings(inst->channels, 16, inst->sample_freq);

    return result;
}

/* Close avrouter configuration */
static CsrBool avrouter_close(void **instance)
{
    avrouter_instance_t *inst   = (avrouter_instance_t*)*instance;

    if(inst->file)
    {
        CsrBtFclose(inst->file);
        inst->file = NULL;
    }
    
    return TRUE;
}

/* Start avrouter streaming */
static CsrBool avrouter_start(void **instance)
{
    avrouter_instance_t *inst = (avrouter_instance_t*)*instance;
    CsrAv2DspmStreamReadyToStart(inst->avInstance);
    return TRUE;
}

/* Stop avrouter streaming */
static CsrBool avrouter_stop(void **instance)
{
    avrouter_instance_t *inst = (avrouter_instance_t*)*instance;
    /* Close dspm stream */
    CsrBtAv2DspmSendDisconnectStreams(inst->avInstance);
    return TRUE;
}

/* Process avrouter data: Empty windows buffers */
static CsrBool avrouter_process(CsrUint8 index,
                            av2filter_t **chain,
                            CsrBool freeData,
                            void *data,
                            CsrUint32 length)
{
    avrouter_instance_t *inst = (avrouter_instance_t*)(chain[index]->f_instance);
    CsrUint32            wr;
    CsrUint32            paysize;


    /* Write data to file */
    paysize = CSR_BT_AV_FIXED_MEDIA_PACKET_HDR_SIZE+1;
    if(length > paysize)
    {
        wr = CsrBtFwrite((char*)data + (CSR_BT_AV_FIXED_MEDIA_PACKET_HDR_SIZE + 1),
                       1,
                       (length - paysize),
                       inst->file);
        if(wr == 0)
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,AVROUTER_PRN "file write error, code %i\n",
                   ferror(inst->file));
        }
        else if(wr != (length - paysize))
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,AVROUTER_PRN "%i bytes received, but only %i written\n",
                   length, wr);
        }
    }

    /* Just invoke next stage */
    return chain[index+1]->f_process((CsrUint8)(index+1),
                                     chain,
                                     freeData,
                                     data,
                                     length);
}

/* Return avrouter configuration */
static CsrUint8 *avrouter_get_config(void **instance,
                                 CsrUint8 *conf_len)
{
    CsrUint8 tmp;
    CsrUint8 *conf;
    avrouter_instance_t *inst;
    AvDspmType *dspmInst;

#ifdef DEBUG
    printf(AVROUTER_PRN "get_config\n");
#endif

    inst = (avrouter_instance_t*)*instance;
    dspmInst = (AvDspmType *)&inst->avInstance->avCon[inst->avInstance->currentConnection].avDspmInst;
    if (dspmInst->encoderToUse == CSR_DSPM_CAPABILITY_SBC_ENCODER)
    {
        *conf_len = SBC_MEDIA_CODEC_SC_SIZE;
    }
    else
    {
        *conf_len = CSR_APTX_MEDIA_CODEC_SC_SIZE;
    }
    conf = CsrPmemAlloc(*conf_len);

    conf[0] = CSR_BT_AV_SC_MEDIA_CODEC;
    conf[2] = CSR_BT_AV_AUDIO<<4;
    

    /* Sampling rate */
    switch(inst->sample_freq)
    {
        case 48000:
            tmp = 0x10;
            break;

        case 44100:
            tmp = 0x20;
            break;

        case 32000:
            tmp = 0x40;
            break;

        default:
            tmp = 0x80;
            break;
    }

    if (dspmInst->encoderToUse == CSR_DSPM_CAPABILITY_SBC_ENCODER)
    {
        conf[1] = SBC_MEDIA_CODEC_SC_SIZE - 2;
        conf[3] = CSR_BT_AV_SBC;

        /* Channel mode */
        conf[4] = (0x08>>inst->channel_mode) | tmp;

        /* Allocation */
        tmp = (inst->alloc_method == CSR_SBC_METHOD_SNR) ? 0x02 : 0x01;

        /* Blocks, allocation and subbands */
        conf[5] = ((0x80>>((inst->blocks>>2)-1)) |
                   (0x08>>((inst->subbands>>2)-1)) |
                   tmp);

        /* Bitpools */
        conf[6] = inst->minBitpool;
        conf[7] = inst->maxBitpool;
    }
    else
    { /* APT-X */
        conf[1] = CSR_APTX_MEDIA_CODEC_SC_SIZE-2;
        conf[3] = CSR_BT_AV_NON_A2DP_CODEC;
        /* Vendor Id */
        conf[4] = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x000000FF);
        conf[5] = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x0000FF00);
        conf[6] = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x00FF0000);
        conf[7] = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0xFF000000);
        /* Codec Id */
        conf[8] = (CsrUint8)(CSR_APTX_CODEC_ID & 0x00FF);
        conf[9] = (CsrUint8)(CSR_APTX_CODEC_ID & 0xFF00);
        conf[10] = (0x08>>inst->channel_mode) | tmp; /* sample rate and channel mode */
    }

    return conf;
}

/* Set avrouter configuration */
static CsrBtAvResult avrouter_set_config(void **instance,
                                    CsrUint8 *conf,
                                    CsrUint8 conf_len)
{
    avrouter_instance_t *inst;
    inst = (avrouter_instance_t*)*instance;

#ifdef DEBUG
    printf(AVROUTER_PRN "set_config\n");
#endif

    /* Dummy check that service capabilities are SBC */
    if(!(*(conf+2)>>4 == CSR_BT_AV_AUDIO) ||
       ((*(conf+3) != CSR_BT_AV_SBC) && (*(conf+3) != CSR_BT_AV_NON_A2DP_CODEC)))
    {
#ifdef DEBUG
        printf(AVROUTER_PRN "set_config: unsupported config detected\n");
#endif
        return CSR_BT_RESULT_CODE_A2DP_UNSUPPORTED_CONFIGURATION;
    }

    if (*(conf+3) == CSR_BT_AV_SBC)
    {/* Store SBC configuration */
        inst->remoteMinBitpool = *(conf+6);
        inst->remoteMaxBitpool = *(conf+7);
        /* Perform a reconfiguration */
        avrouter_store_config(inst, conf+4);
    }
    else
    {
        if ((*(conf+4) == (CsrUint8)(CSR_APTX_VENDOR_ID & 0x000000FF) ||
             *(conf+4) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x000000FF)) &&
             *(conf+5) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x0000FF00) &&
             *(conf+6) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x00FF0000) &&
             *(conf+7) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0xFF000000) ) 
        {/* APTX-X */
            (void)avrouter_store_aptx_config(inst, conf+10);
        }
        else
        {
#ifdef DEBUG
            printf(AVROUTER_PRN "set_config: unsupported config detected\n");
#endif
            return CSR_BT_RESULT_CODE_A2DP_UNSUPPORTED_CONFIGURATION;
        }
    }

    
    return CSR_BT_AV_ACCEPT;
}

/* Return avrouter service capabilities */
static CsrUint8 *avrouter_get_caps(void **instance,
                               CsrUint8 *cap_len)
{
    CsrUint8 *cap;
    CsrUint8 i;
    avrouter_instance_t *inst;
    AvDspmType *dspmInst;
    inst = (avrouter_instance_t*)*instance;
    dspmInst = (AvDspmType *)&inst->avInstance->avCon[inst->avInstance->currentConnection].avDspmInst;
    if (dspmInst->encoderToUse == CSR_DSPM_CAPABILITY_SBC_ENCODER)
    {
        *cap_len = SBC_MEDIA_CODEC_SC_SIZE + 2;
    }
    else
    {
        *cap_len = CSR_APTX_MEDIA_CODEC_SC_SIZE + 2;
    }

#ifdef DEBUG
    printf(AVROUTER_PRN "remote_caps\n");
#endif

    cap = CsrPmemAlloc(*cap_len);

    cap[0] = CSR_BT_AV_SC_MEDIA_TRANSPORT;
    cap[1] = 0;
    cap[2] = CSR_BT_AV_SC_MEDIA_CODEC;
    cap[4] = CSR_BT_AV_AUDIO<<4;

    if (dspmInst->encoderToUse == CSR_DSPM_CAPABILITY_SBC_ENCODER)
    {
        cap[3] = SBC_MEDIA_CODEC_SC_SIZE - 2;
        cap[5] = CSR_BT_AV_SBC;
        for(i=0; i<4; i++)
        {
            cap[6+i] = inst->sbc_capabilities[i];
        }
    }
    else
    {/* APT-X */
        CsrUint8 tmp;
        /* Sampling rate */
        switch(inst->sample_freq)
        {
            case 48000:
                tmp = 0x10;
                break;

            case 44100:
                tmp = 0x20;
                break;

            case 32000:
                tmp = 0x40;
                break;

            default:
                tmp = 0x80;
                break;
        }
        cap[3]  = CSR_APTX_MEDIA_CODEC_SC_SIZE - 2;
        cap[5]  = CSR_BT_AV_NON_A2DP_CODEC;
        cap[6]  = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x000000FF);
        cap[7]  = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x0000FF00);
        cap[8]  = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x00FF0000);
        cap[9]  = (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0xFF0000);
        cap[10] = (CsrUint8)(CSR_APTX_CODEC_ID & 0x00FF);
        cap[11] = (CsrUint8)(CSR_APTX_CODEC_ID & 0xFF00);
        cap[12] = (0x08>>inst->channel_mode) | tmp;
    }
    return cap;
}

/* Investigate remote capabilities, return optimal configuration */
static CsrUint8 *avrouter_remote_caps(void **instance,
                                  CsrUint8 *caps,
                                  CsrUint8 caps_len,
                                  CsrUint8 *conf_len)
{
    CsrUint8 i;
    CsrUint8 *conf;
    avrouter_instance_t *inst;
    AvDspmType *dspmInst;
    inst = (avrouter_instance_t*)*instance;
    dspmInst = (AvDspmType *)&inst->avInstance->avCon[inst->avInstance->currentConnection].avDspmInst;

#ifdef DEBUG
    printf(AVROUTER_PRN "remote_caps\n");
#endif

    if (*(caps+3) == CSR_BT_AV_SBC)
    {
        *conf_len = SBC_MEDIA_CODEC_SC_SIZE;
    }
    else 
    {
        *conf_len = CSR_APTX_MEDIA_CODEC_SC_SIZE;
    }
    

    /* Dummy check that service capabilities are SBC */
    if(!(*(caps+2)>>4 == CSR_BT_AV_AUDIO) ||
       ((*(caps+3) != CSR_BT_AV_SBC) && (*(caps+3) != CSR_BT_AV_NON_A2DP_CODEC)) )
    {
        *conf_len = 0;
        return NULL;
    }

    conf = (CsrUint8*)CsrPmemAlloc(*conf_len);
    /* Common codec service capability */
    conf[0] = CSR_BT_AV_SC_MEDIA_CODEC;
    conf[2] = CSR_BT_AV_AUDIO<<4;

    if ((*(caps+3) == CSR_BT_AV_SBC) 
#ifdef CSR_USE_DSPM
        && (dspmInst->encoderToUse == CSR_DSPM_CAPABILITY_SBC_ENCODER)
#endif
        )
    {/* Store remote bitpools to use in reconfig */
        inst->minBitpool = *(caps+6);
        inst->maxBitpool = *(caps+7);
        inst->remoteMinBitpool = *(caps+6);
        inst->remoteMaxBitpool = *(caps+7);

        
        conf[1] = SBC_MEDIA_CODEC_SC_SIZE - 2;
        
        conf[3] = CSR_BT_AV_SBC;

        /* SBC specific: Sampling frequency */
        for(i=0; i<4; i++)
        {
            /* ...a workaround for Sonorix (has very bad sound unless using 32kHz) */
            if((*(caps+4) == 0x6a) && (*(caps+7) == 0x2c))
            {
                conf[4] = 0x40;
                break;
            }
            if(*(caps+4) & (0x10<<i) & inst->sbc_capabilities[0])
            {
                conf[4] = (0x10<<i);
                break;
            }
        }

        /* SBC specific: Channel mode */
        for(i=0; i<4; i++)
        {
            if(*(caps+4) & (0x01<<i) & inst->sbc_capabilities[0])
            {
                conf[4] |= (0x01<<i);
                break;
            }
        }

        /* SBC specific: Block length */
        for( i=0; i<4; i++)
        {
            if(*(caps+5) & (0x10<<i) & inst->sbc_capabilities[1])
            {
                conf[5] = (0x10<<i);
                break;
            }
        }

        /* SBC specific: Subbands */
        for(i=0; i<2; i++)
        {
            if(*(caps+5) & (0x04<<i) & inst->sbc_capabilities[1])
            {
                conf[5] |= (0x04<<i);
                break;
            }
        }

        /* SBC specific: Allocation method */
        for(i=0; i<2; i++)
        {
            if(*(caps+5) & (0x01<<i) & inst->sbc_capabilities[1])
            {
                conf[5] |= (0x01<<i);
                break;
            }
        }

        /* SBC specific: Min and max bitpool */
        conf[6] = inst->minBitpool;
        conf[7] = inst->maxBitpool;

        /* Store calculated configuration to be able to reconfig */
        avrouter_store_config(inst, conf+4);
    }
    else
    {/* Use APT-X as the next encoder: check whether the remote capabilities include APT-X */
        if ((*(caps+4) == (CsrUint8)(CSR_APTX_VENDOR_ID & 0x000000FF) ||
            *(caps+4) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x000000FF)) &&
            *(caps+5) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x0000FF00) &&
            *(caps+6) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0x00FF0000) &&
            *(caps+7) == (CsrUint8)(CSR_APTX_CSR_VENDOR_ID & 0xFF000000) ) 
        {/* APTX-X */
            conf[1] = CSR_APTX_MEDIA_CODEC_SC_SIZE - 2;
            conf[3] = CSR_BT_AV_NON_A2DP_CODEC;
            CsrMemCpy(&conf[4], (caps+4), sizeof(CsrUint32));
            conf[8]  = *(caps+8);
            conf[9]  = *(caps+9);
            conf[10] = avrouter_store_aptx_config(inst, (caps+10));
        }
        else
        {
            *conf_len = 0;
            return NULL;
        }
    }

    return conf;
}

/* Sink buffer level update */
static void avrouter_qos_update(void **instance,
                             CsrUint16 qosLevel)
{
    /* Not supported */
}

/* Print menu options to screen */
static void avrouter_menu(void **instance,
                       CsrUint8 *num_options)
{
    /* Not supported */
    printf("  AVROUTER filter does not support menu options\n");
    *num_options = 0;
}

/* Set user option */
static CsrBool avrouter_setup(void **instance,
                          CsrUint8 index,
                          char *value)
{
    /* Not supported */
    if(value)
    {
        CsrPmemFree(value);
    }
    return FALSE;
}


static char* avrouter_return_filename(void **instance)
{
    return NULL;
}


