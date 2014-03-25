/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <signal.h>


#include "csr_bt_av_lib.h"
#include "csr_bt_av_prim.h"
#include "Bluetooth_AV.h"
#include "Bluetooth_Common.h"
#include "csr_bt_av2_util.h"

#include "csr_app_lib.h"
#include "csr_samsung_log.h"

#define DEBUG
/* Settings */
#define ALSA_PRN            "alsa: "

#define BUFFER_SIZE         8192               /* Size of each buffer in bytes */
#define BUFFERS             32                 /* Number of buffers */
#define PLAYBACK_THRESHOLD  24                 /* Have this many filled buffers before attempting restart */
#define BUFFER_THRESHOLD    (7/8)              /* Wait until this percentage of BUFFER_SIZE has been received before playback */
#define READ_FRAMES         1024               /* Try to capture this many frames (apply framesize to get size in bytes) */
#define CAPTURE_THRESHOLD   12288              /* Collect capture sample buffers of this size */
#define PERIODS             4                  /* Number of ALSA driver buffers */
#define PERIOD_SIZE         8192               /* Size of each ALSA driver buffer */

/* #define DEBUG */
/* #define VERBOSE_DEBUG */

typedef struct
{
    av2filter_t           *filter;
    char                  *device;
    snd_pcm_sframes_t     framesize;
    snd_pcm_stream_t      stream;
    snd_pcm_t             *handle;
    snd_pcm_hw_params_t   *params;
    CsrUint32              sample_rate;
    CsrUint32              channels;

    CsrBool                run;
    pthread_mutex_t       mutex;
    pthread_cond_t        wake;
    pthread_t             thread;
    CsrUint32              errors;
    CsrBool                suspend;

    void                  *buffers[BUFFERS];
    CsrUint32              lengths[BUFFERS];
    CsrUint8               buf_in;
    CsrUint8               buf_out;
    CsrUint8               count;

    CsrUint8               *capbuf;
    CsrUint32              capsize;

    CsrSchedBgint             bgint_filter;
} alsa_instance_t;

/* Filter entry prototypes */
static CsrBool alsa_init(void **instance, av2filter_t *filter, void *av2inst);
static CsrBool alsa_deinit(void **instance);
static CsrBool alsa_open(void **instance);
static CsrBool alsa_close(void **instance);
static CsrBool alsa_start(void **instance);
static CsrBool alsa_stop(void **instance);
static CsrBool alsa_process_play(CsrUint8 index,
                                av2filter_t **chain,
                                CsrBool freeData,
                                void *data,
                                CsrUint32 length);
static CsrBool alsa_process_capture(CsrUint8 index,
                                   av2filter_t **chain,
                                   CsrBool freeData,
                                   void *data,
                                   CsrUint32 length);
static CsrUint8 *alsa_get_config(void **instance,
                                CsrUint8 *conf_len);
static CsrBtAvResult alsa_set_config(void **instance,
                                   CsrUint8 *conf,
                                   CsrUint8 conf_len);
static CsrUint8 *alsa_get_caps(void **instance,
                              CsrUint8 *cap_len);
static CsrUint8 *alsa_remote_caps(void **instance,
                                 CsrUint8 *caps,
                                 CsrUint8 caps_len,
                                 CsrUint8 *conf_len);
static void alsa_qos_update(void **instance,
                            CsrUint16 qosLevel);
static void alsa_menu(void **instance,
                      CsrUint8 *num_options);
static CsrBool alsa_setup(void **instance,
                         CsrUint8 index,
                         char *value);

/* Filter structure setup for ALSA playback */
av2filter_t filter_alsa_play =
{
    "csr_linux_alsa_playback",
    FILTER_CONSUMER,
    AV_NO_MEDIA,
    AV_NO_CODEC,
    0,
    NULL,
    alsa_init,
    alsa_deinit,
    alsa_open,
    alsa_close,
    alsa_start,
    alsa_stop,
    alsa_process_play,
    alsa_get_config,
    alsa_set_config,
    alsa_get_caps,
    alsa_remote_caps,
    alsa_qos_update,
    alsa_menu,
    alsa_setup
};

/* Filter structure setup for ALSA capture */
av2filter_t filter_alsa_capture =
{
    "csr_linux_alsa_capture",
    FILTER_PRODUCER,
    AV_NO_MEDIA,
    AV_NO_CODEC,
    0,
    NULL,
    alsa_init,
    alsa_deinit,
    alsa_open,
    alsa_close,
    alsa_start,
    alsa_stop,
    alsa_process_capture,
    alsa_get_config,
    alsa_set_config,
    alsa_get_caps,
    alsa_remote_caps,
    alsa_qos_update,
    alsa_menu,
    alsa_setup
};

/* ALSA xrun handler */
static int alsa_xrun(alsa_instance_t *inst, int err)
{
    int res;

    inst->errors++;
    res = 0;
    switch(err)
    {
        case -EAGAIN:
            if((inst->errors % 10) == 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "device has more data, error count %i\n", inst->errors));
            }
            snd_pcm_wait(inst->handle, 1000);
            break;

        case -EBADFD:
            if((inst->errors % 10) == 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "device not in right state, error count %i\n", inst->errors));
            }
            snd_pcm_prepare(inst->handle);
            break;

        case -EPIPE:
            if((inst->errors % 10) == 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "device over/under-run, error count %i\n", inst->errors));
            }
            snd_pcm_prepare(inst->handle);
            snd_pcm_pause(inst->handle, 1);
            inst->suspend = TRUE;
            break;

        case -ESTRPIPE:
            if((inst->errors % 10) == 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "device suspend error, error count %i\n", inst->errors));
            }
            snd_pcm_prepare(inst->handle);
            break;

        default:
            if((inst->errors % 10) == 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "playback thread, unknown error, code %i, error count %i\n",
                       res, inst->errors));
            }
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

#ifdef DEBUG
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "thread begin...\n"));
#endif

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
            CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "playback device did not consume all frames, sent %i, consumed %i\n",
                   frames, res));
            break;
        }
    }

#ifdef DEBUG
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "thread end...\n"));
#endif

    return NULL;
}

/* ALSA capture thread */
static void *alsa_thread_capture(void *instance)
{
    alsa_instance_t *inst;
    snd_pcm_sframes_t cap;

    inst = (alsa_instance_t*)instance;

    while(inst->run)
    {
        /* Capture frames */
        cap = snd_pcm_readi(inst->handle,
                            inst->buffers[inst->buf_in],
                            READ_FRAMES);
#ifdef VERBOSE_DEBUG
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "read %i frames\n", (int)cap));
#endif

        if(cap < 0)
        {
            /* Over/underruns, don't free buffer */
            alsa_xrun(inst, cap);
        }
        else
        {
            /* Update ringbuffer */
            inst->lengths[inst->buf_in] = cap * inst->framesize;
            inst->buf_in = (inst->buf_in + 1) % BUFFERS;

            pthread_mutex_lock(&inst->mutex);
            inst->count++;
            pthread_mutex_unlock(&inst->mutex);

            /* Wake up processer */
            CsrSchedBgintSet(inst->bgint_filter);
        }
    }

    return NULL;
}

/* Initialise alsa filter */
static CsrBool alsa_init(void **instance, struct av2filter_t *filter, void *av2inst)
{
    alsa_instance_t *inst;
    CsrUint8 i;
    *instance = CsrPmemZalloc(sizeof(alsa_instance_t));
    inst = (alsa_instance_t*)*instance;

    /* Allocate things */
    snd_pcm_hw_params_malloc(&(inst->params));
    if(inst->params == NULL)
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not allocate ALSA hw_params structure\n"));
        return FALSE;
    }
#ifdef DEBUG
    else
    {
        CsrMemSet(inst->params, 0, snd_pcm_hw_params_sizeof());
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "allocated hw_params structure\n"));
    }
#endif

    /* Set defaults */
    inst->device = strdup(CSR_BT_AV_ALSA_DEFAULT_DEVICE);
    inst->filter = filter;
    inst->stream = ((filter->f_type & FILTER_CONSUMER)
                    ? SND_PCM_STREAM_PLAYBACK
                    : SND_PCM_STREAM_CAPTURE);

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
    inst->bgint_filter = CsrSchedBgintReg(filtersBackgroundInterrupt, av2inst, "alsa_filter");

    /* Initialise transfer buffer */
    pthread_mutex_init(&inst->mutex, NULL);
    pthread_cond_init(&inst->wake, NULL);

#ifdef DEBUG
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "alsa filter initialized\n"));
#endif

    return TRUE;
}

/* Deinitialise alsa filter */
static CsrBool alsa_deinit(void **instance)
{
    alsa_instance_t *inst;
    CsrUint8 i;
    inst = (alsa_instance_t*)*instance;

    CsrSchedBgintUnreg(inst->bgint_filter);

    /* Free members */
    if(inst->params != NULL)
    {
        snd_pcm_hw_params_free(inst->params);
    }
    if(inst->device != NULL)
    {
        free(inst->device);
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

#ifdef DEBUG
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "opening alsa filter\n"));
#endif

    /* Close before reopen */
    result = TRUE;
    if(inst->handle != 0)
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "filter already open - reopening\n"));
        result = alsa_close(instance);
    }

    /* Get global AV2 configuration */
    getPcmSettings(&chans, &bits, &freq);
    inst->channels = chans;
    inst->sample_rate = freq;

    /* Open PCM device */
    if(result && (snd_pcm_open(&(inst->handle),
                               inst->device,
                               inst->stream,
                               0) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not open PCM device '%s'\n", inst->device));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "pcm device opened, 0x%08x\n", inst->handle));
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "pcm device params, 0x%08x\n", inst->params));
    }
#endif


    /* Get config */
    if(result && (snd_pcm_hw_params_any(inst->handle,
                                        inst->params) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not get hardware parameters\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "got hw parameters\n"));
    }
#endif


    /* Set access mode */
    if(result && (snd_pcm_hw_params_set_access(inst->handle,
                                               inst->params,
                                               SND_PCM_ACCESS_RW_INTERLEAVED) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set access mode\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "access mode set\n"));
    }
#endif

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
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set PCM format (signed 16bit little endian)\n"));
#else
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set PCM format (signed 16bit big endian)\n"));
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
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set sample rate\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "sample rate set\n");
    }
#endif

    if(result && (tmp != inst->sample_rate))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "sample rate is not supported by the hardware\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "actual sample rate matches requested rate\n"));
    }
#endif


    /* Channels */
    if(result && (snd_pcm_hw_params_set_channels(inst->handle,
                                                 inst->params,
                                                 inst->channels) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set number of channels\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "number of channels set\n"));
    }
#endif


    /* Periods (buffers/fragments) */
    if(result && (snd_pcm_hw_params_set_periods(inst->handle,
                                                inst->params,
                                                PERIODS,
                                                0) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set number of periods\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "periods set\n"));
    }
#endif


    /* Set buffer size. We alsays use 16 bit (2 byte) mode, but we must also take
     * account for the number of channels */
    inst->framesize = 2 * inst->channels;
    tmp = PERIOD_SIZE * PERIODS / inst->framesize;
    if(result && (snd_pcm_hw_params_set_buffer_size(inst->handle,
                                                    inst->params,
                                                    tmp) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set buffer size (%i)\n", tmp));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "buffer size set\n"));
    }
#endif


    /* Apply parameters */
    if(result && (snd_pcm_hw_params(inst->handle,
                                    inst->params) < 0))
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not set hw parameters\n"));
        result = FALSE;
    }
#ifdef VERBOSE_DEBUG
    else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "parameters applied\n"));
    }
#endif

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
            CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "error closing PCM handle, code %i\n", code));
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
#ifdef DEBUG
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "preparing pcm\n"));
#endif
    code = snd_pcm_prepare(inst->handle);
    if(code < 0)
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "error preparing PCM handle, code %i\n", code));
        result = FALSE;
    }
    else
    {
#ifdef DEBUG
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "starting thread\n"));
#endif
        inst->errors = 0;
        inst->run = TRUE;

        /* Start thread */
        if(inst->filter->f_type & FILTER_CONSUMER)
        {
            /* Playback thread */
            code = pthread_create(&inst->thread,
                                  NULL,
                                  alsa_thread_play,
                                  inst);
            if(code != 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not start playback thread, code %i\n", code));
                result = FALSE;
            }
            else
            {
                result = TRUE;
#ifdef DEBUG
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "playback thread started\n"));
#endif

            }
        }
        else
        {
            /* Capture thread */
            code = pthread_create(&inst->thread,
                                  NULL,
                                  alsa_thread_capture,
                                  inst);
            if(code != 0)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "could not start capture thread, code %i\n", code));
                result = FALSE;
            }
            else
            {
                result = TRUE;
#ifdef DEBUG
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "capture thread started\n"));
#endif
            }
        }
    }

#ifdef DEBUG
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "PCM device now streaming\n"));
#endif

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
    if(inst->thread != 0)
    {
        pthread_cond_broadcast(&inst->wake);
        code = pthread_join(inst->thread, &thres);
        if(code != 0)
        {
            CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "error stopping thread, code %i\n", code));
        }
		
        inst->thread = 0;
    }
	else
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "thread not running\n"));
		      result = TRUE;
    }
	/* Drain the handle */
    code = snd_pcm_drain(inst->handle);
    if(code < 0)
    {
        CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "error draining (stopping) PCM handle, code %i\n", code));
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }
   

    return result;
}

/* Process alsa playback data */
static CsrBool alsa_process_play(CsrUint8 index,
                                av2filter_t **chain,
                                CsrBool freeData,
                                void *data,
                                CsrUint32 length)
{
    alsa_instance_t *inst;
    inst = (alsa_instance_t*)(chain[index]->f_instance);

    /* Put data into incoming buffer */
    CsrMemCpy(inst->buffers[inst->buf_in] + inst->lengths[inst->buf_in],
           data,
           length);
    inst->lengths[inst->buf_in] += length;

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
    if(freeData && data)
    {
        CsrPmemFree(data);
    }

    /* Pass data on to next stage */
    return chain[index+1]->f_process(index+1,
                                     chain,
                                     FALSE,
                                     NULL,
                                     0);
}

/* Process ALSA capture data. This function is called on behalf of
 * bgint7, so we currently hold the scheduler context */
static CsrBool alsa_process_capture(CsrUint8 index,
                                   av2filter_t **chain,
                                   CsrBool freeData,
                                   void *data,
                                   CsrUint32 length)
{
    alsa_instance_t *inst;
    CsrUint32 copy;
    CsrBool result;
    inst = (alsa_instance_t*)(chain[index]->f_instance);

    result = TRUE;
    while(result)
    {
        /* Is there an item ready? */
        pthread_mutex_lock(&inst->mutex);
        if(inst->count > 0)
        {
            inst->count--;
            pthread_mutex_unlock(&inst->mutex);
        }
        else
        {
            pthread_mutex_unlock(&inst->mutex);
            result = FALSE;
        }

        /* This is the candidate data */
        if(result)
        {
            /* Copy as much as possible */
            copy = CSRMAX(CAPTURE_THRESHOLD - inst->capsize,
                       inst->lengths[inst->buf_out]);
            CsrMemCpy(inst->capbuf + inst->capsize,
                   inst->buffers[inst->buf_out],
                   copy);
            inst->capsize += copy;

            /* If we got enough data, send it */
            if(inst->capsize >= CAPTURE_THRESHOLD)
            {
                CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "delivering %i bytes\n", inst->capsize));
                result &= chain[index+1]->f_process(index+1,
                                                    chain,
                                                    FALSE,
                                                    inst->capbuf,
                                                    inst->capsize);
                inst->capsize = 0;
            }

            /* Anything left in the ring buffer? */
            if(copy < inst->lengths[inst->buf_out])
            {
                CsrMemCpy(inst->capbuf,
                       inst->buffers[inst->buf_out] + copy,
                       inst->lengths[inst->buf_out] - copy);
                inst->capsize = inst->lengths[inst->buf_out] - copy;
            }

            /* Get ready for next buffer */
            inst->buf_out = (inst->buf_out + 1) % BUFFERS;

#ifdef VERBOSE_DEBUG
            CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "process done send\n"));
#endif
        }
    }

    /* Done */
    {
        static int foo = 0;
        foo++;
        if(foo > 10)
        {
            CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,ALSA_PRN "capture count %i\n", inst->count));
            foo = 0;
        }
    }
    return result;
}

/* Return alsa configuration */
static CsrUint8 *alsa_get_config(void **instance,
                                CsrUint8 *conf_len)
{
    /* We do not support this feature */
    *conf_len = 0;
    return NULL;
}

/* Set alsa configuration */
static CsrBtAvResult alsa_set_config(void **instance,
                                   CsrUint8 *conf,
                                   CsrUint8 conf_len)
{
    /* We do not support this feature */
    return CSR_BT_AV_ACCEPT;
}

/* Return alsa service capabilities */
static CsrUint8 *alsa_get_caps(void **instance,
                              CsrUint8 *cap_len)
{
    /* We do not support this feature */
    *cap_len = 0;
    return NULL;
}

/* Investigate remote capabilities, return optimal configuration */
static CsrUint8 *alsa_remote_caps(void **instance,
                                 CsrUint8 *caps,
                                 CsrUint8 caps_len,
                                 CsrUint8 *conf_len)
{
    /* We do not support this feature */
    *conf_len = 0;
    return NULL;
}

static void alsa_qos_update(void **instance,
                            CsrUint16 qosLevel)
{
    /* Not supported */
}

/* Print menu options to screen */
static void alsa_menu(void **instance,
                      CsrUint8 *num_options)
{
    /* Not supported */
    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"  ALSA playback/capture does not support menu options\n"));
    *num_options = 0;
}

/* Set user option */
static CsrBool alsa_setup(void **instance,
                         CsrUint8 index,
                         char *value)
{
    /* Not supported */
    CsrPmemFree(value);
    return FALSE;
}
