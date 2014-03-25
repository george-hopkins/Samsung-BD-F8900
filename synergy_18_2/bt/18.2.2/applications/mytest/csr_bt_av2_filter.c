/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_av_lib.h"
#include "csr_bt_av_prim.h"
#include "Bluetooth_Common.h"
#include "Bluetooth_AV.h"
#include "csr_bt_av2_util.h"
#include "Bluetooth_Common.h"



#define DEBUG          1
#define DEBUG_VERBOSE  1


/* Externally defined filter definitions */


#ifdef USE_WINAUDIO
extern av2filter_t filter_winplay;
//extern av2filter_t filter_wingrab;
#endif

#ifdef USE_ALSA
extern av2filter_t filter_alsa_play;
extern av2filter_t filter_alsa_capture;
#endif

#ifdef USE_SBC
extern av2filter_t filter_sbc_encode;
extern av2filter_t filter_sbc_decode;
#endif
extern av2filter_t filter_ss_encode; 

#ifdef USE_SBCFILE
extern av2filter_t filter_sbcfile_streamer;
extern av2filter_t filter_sbcfile_writer;
#endif

#ifdef USE_FILEDUMP
extern av2filter_t filter_filedump;
#endif

#ifdef USE_MP3
extern av2filter_t filter_mp3;
#endif

#ifdef USE_AAC
extern av2filter_t filter_aac;
#endif

#ifdef USE_WAVE
extern av2filter_t filter_stream_wave;
extern av2filter_t filter_write_wave;
#endif

#ifdef USE_AVROUTER
extern av2filter_t filter_avrouter_encode;
extern av2filter_t filter_avrouter_decode;
#endif

/* Special AV2 filters */
//extern av2filter_t av2_source_filter;
extern av2filter_t av2_source_filter;

/**************************************************************************************************
 * filtersSetup
 **************************************************************************************************/
void filtersSetup(Av2AppInst *instData)
{

}

/**************************************************************************************************
 * filtersInstall
 **************************************************************************************************/
void filtersInstall(Av2AppInst *instData, CsrUint8 installedCodec)
{
	CsrUint8 idx;
   if( instData->installedCodec !=installedCodec)
   {
	/* This is the default SINK filter chain */
		idx = 0;
	    if( installedCodec==SBC_CODEC_TYPE)
	    {

	CL_API_LOGPrintf("filtersInstall SBC_CODEC_TYPE\n");

		instData->installedCodec=SBC_CODEC_TYPE;
#ifdef USE_SBC
		instData->filters[idx++] = &filter_sbc_encode;
#endif

#ifdef USE_SBCFILE
		instData->filters[idx++] = &filter_sbcfile_streamer;
#endif
	    }  
	    else
	    {
           CL_API_LOGPrintf("filtersInstall SS_CODEC_TYPE\n");
	    
	       instData->installedCodec=SS_CODEC_TYPE;
		   instData->filters[ idx ++]= &filter_ss_encode;	
	    }

	
#ifdef USE_WAVE
		instData->filters[idx++] = &filter_stream_wave;
#endif
#ifdef USE_WINAUDIO
		instData->filters[idx++] = &filter_wingrab;
#endif
#ifdef USE_ALSA
		instData->filters[idx++] = &filter_alsa_capture;
#endif
#ifdef USE_FILEDUMP
		instData->filters[idx++] = &filter_filedump;
#endif
#ifdef USE_AVROUTER
		instData->filters[idx++] = &filter_avrouter_encode;
#endif
	
		instData->filters[idx++] = &av2_source_filter;   /* Internal filter for stopping chain */

		instData->filter_count = idx;
		instData->filters[idx++] = NULL;			   /* Terminate list */
		
			/* Check consistency of filters */
		if(filtersCheck(instData))
		{
#ifdef DEBUG
			CL_API_LOGPrintf("Filter combinations seems to be good\n");
#endif
		}
		else
		{
			CL_API_LOGPrintf("Filter combinations are invalid, check 'csr_bt_av2_filter.c' function filtersSetup()\n");
		}
   	}

}

/**************************************************************************************************
 * filterBackgroundInterrupt
 **************************************************************************************************/
void filtersBackgroundInterrupt(void *arg)
{
    Av2AppInst *av2inst;

    av2inst = (Av2AppInst *)arg;

    /* Invoke first filter */
    av2inst->filters[0]->f_process(0,
                                       av2inst->filters,
                                       FALSE,
                                       NULL,
                                       0);
}

/**************************************************************************************************
 * filterTimerInterrupt
 **************************************************************************************************/
void filtersTimerInterrupt(CsrUint16 mi, void *mv)
{
    extern Av2AppInst *Av2Instance;
    CsrTime now;
    CsrInt32 adj;
    CsrInt32 last     = Av2Instance->filter_lasttime;
    CsrInt32 delay    = Av2Instance->filter_delay;
    CsrInt32 interval = Av2Instance->filter_interval;

    /* Reschedule ourself, taking elapsed time into account */
    now = CsrTimeGet(NULL);
    adj = (now - last) - delay;

    /* Secure timer calculation */
    if((interval - adj) < (CsrInt32)(CSR_SCHED_MILLISECOND))
    {
        delay = (CsrInt32)(CSR_SCHED_MILLISECOND);
#ifdef DEBUG
        CL_API_LOGPrintf("\nTimer too slow compensation:\n   now:%08lu last:%08lu interval:%08lu delay:%06lu adjust:%i\n",
               now, last, interval, delay, adj);
#endif
    }
    else
    {
        delay = interval - adj;
#ifdef DEBUG_VERBOSE
        CL_API_LOGPrintf("\nTimer compensation, now:%08lu last:%08lu interval:%08lu delay:%08lu adjust:%i\n",
               now, last, interval, delay, adj);
#endif
    }

    Av2Instance->filter_delay    = delay;
    Av2Instance->filter_lasttime = now;

    /* Schedule another timed event _before_ we invoke filter so that
     * the processing has as little influence as possible */
    if((Av2Instance->filter_timer != 0) &&
       (Av2Instance->filter_delay > 0))
    {
        Av2Instance->filter_timer = CsrSchedTimerSet(Av2Instance->filter_delay,
                                                   filtersTimerInterrupt,
                                                   mi,
                                                   mv);
    }
    else
    {
        Av2Instance->filter_timer = 0;
        Av2Instance->filter_delay = 0;
        Av2Instance->filter_interval = 0;
        Av2Instance->filter_lasttime = 0;
    }

    /* Invoke first filter by calling the filter directly */
    Av2Instance->filters[0]->f_process(0,
                                       Av2Instance->filters,
                                       FALSE,
                                       NULL,
                                       0);
}

/**************************************************************************************************
 * filtersStartTimer
 **************************************************************************************************/
void filtersStartTimer(CsrTime interval)
{
    extern Av2AppInst *Av2Instance;

    Av2Instance->filter_interval = interval;

    if(Av2Instance->filter_timer == 0)
    {
        Av2Instance->filter_delay = interval;
        Av2Instance->filter_lasttime = CsrTimeGet(NULL);
        CL_API_LOGPrintf("Periodic timer started, interval %i usec\n", Av2Instance->filter_delay);

        /* Start filter if not runnig, otherwise let it trigger and the new
         * interval will then be used */
        Av2Instance->filter_timer = CsrSchedTimerSet(Av2Instance->filter_delay,
                                                   filtersTimerInterrupt,
                                                   0,
                                                   NULL);
    }
}
/**************************************************************************************************
 * filtersStopTimer
 **************************************************************************************************/
void filtersStopTimer()
{
    extern Av2AppInst *Av2Instance;
    CsrUint16 mi;
    void *mv;

    if(Av2Instance->filter_timer != 0)
    {
        CsrSchedTimerCancel(Av2Instance->filter_timer,
                           &mi,
                           &mv);
        Av2Instance->filter_timer = 0;
        Av2Instance->filter_delay = 0;
        Av2Instance->filter_interval = 0;
        Av2Instance->filter_lasttime = 0;
        CL_API_LOGPrintf("Periodic timer stopped\n");
    }
}

/**************************************************************************************************
 * filtersKill
 **************************************************************************************************/
void filtersKill(Av2AppInst *instData)
{
    CsrUint8 idx;

    CL_API_LOGPrintf("Deinitialising filters...\n");
    for(idx=0; idx<instData->filter_count; idx++)
    {
        /* Try going from start -> open */
        if(instData->filters[idx]->f_state == FILTER_ST_START)
        {
            instData->filters[idx]->f_stop(&(instData->filter_instance[idx]));
            instData->filters[idx]->f_state = FILTER_ST_OPEN;
#ifdef DEBUG
            CL_API_LOGPrintf("Filter %i: %15s (%s)\n",
                   idx, "start->open", instData->filters[idx]->f_name);
#endif
        }
        /* Try going from open -> DaAvStateAppActive */
        if(instData->filters[idx]->f_state == FILTER_ST_OPEN)
        {
            instData->filters[idx]->f_close(&(instData->filter_instance[idx]));
            instData->filters[idx]->f_state = FILTER_ST_IDLE;
#ifdef DEBUG
            CL_API_LOGPrintf("Filter %i: %15s (%s)\n",
                   idx, "open->DaAvStateAppActive", instData->filters[idx]->f_name);
#endif
        }
        /* Try going from DaAvStateAppActive -> un-initialised */
        if(instData->filters[idx]->f_state == FILTER_ST_IDLE)
        {
            instData->filters[idx]->f_close(&(instData->filter_instance[idx]));
            instData->filters[idx]->f_state = FILTER_ST_NOINIT;
#ifdef DEBUG
            CL_API_LOGPrintf("Filter %i: %15s (%s)\n",
                   idx, "DaAvStateAppActive->noinit", instData->filters[idx]->f_name);
#endif
        }
    }
#ifdef DEBUG
    CL_API_LOGPrintf("Filters killed\n");
#endif
}


/**************************************************************************************************
 * filtersRun
 **************************************************************************************************/
CsrBool filtersRun(Av2AppInst *instData, CsrUint8 op)
{
    CsrInt8 i;
    CsrBool result;
    CsrBool stateok;
    CsrUint8 *state;
    char *name;

    /* If we are in multi-source mode and both connections are up, do not
     * run for the _second_ connection */


    /* Run through all filters */
    state = NULL;
    result = TRUE;
    stateok = TRUE;
    name = "n/a";
#ifdef DEBUG	
	CL_API_LOGPrintf("filtersRun %d\n" ,op);
#endif
    for(i=0;
        (i < instData->filter_count) && result && stateok;
        i++)
    {
        name = instData->filters[i]->f_name;
        state = &(instData->filters[i]->f_state);
        stateok = FALSE;

        switch(op)
        {
            case FILTER_INIT:
                /* Ignore same state */
                if(*state == FILTER_ST_IDLE)
                {
                    stateok = TRUE;
                }

                /* Go from un-initialised to DaAvStateAppActive */
                if(*state == FILTER_ST_NOINIT)
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter init: Index %i: %s\n", i, name);
#endif
                    result = instData->filters[i]->f_init(&(instData->filter_instance[i]),
                                                          (struct av2filter_t*)instData->filters[i],
                                                          (struct Av2AppInst*)instData);
                    instData->filters[i]->f_instance = instData->filter_instance[i];
                    *state = FILTER_ST_IDLE;
                }
                break;

            case FILTER_DEINIT:
                /* Ignore same state */
                if(*state == FILTER_ST_NOINIT)
                {
                    stateok = TRUE;
                }

                /* Go from DaAvStateAppActive to un-initialised */
                if(*state == FILTER_ST_IDLE)
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter deinit: Index %i: %s\n", i, name);
#endif
                    result = instData->filters[i]->f_deinit(&(instData->filter_instance[i]));
                    instData->filters[i]->f_instance = instData->filter_instance[i];
                    *state = FILTER_ST_NOINIT;
                }
                break;

            case FILTER_OPEN:
                /* Ignore same state */
                if(*state == FILTER_ST_OPEN)
                {
                    stateok = TRUE;
                }

                /* Go from open or DaAvStateAppActive to open */
                if((*state == FILTER_ST_IDLE) ||
                   (*state == FILTER_ST_OPEN))
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter open: Index %i: %s\n", i, name);
#endif
                    result = instData->filters[i]->f_open(&(instData->filter_instance[i]));
                    *state = FILTER_ST_OPEN;
                }
                break;

            case FILTER_CLOSE:
                /* Ignore same state */
                if(*state == FILTER_ST_IDLE)
                {
                    stateok = TRUE;
                }

                /* Go from started to open (moves on to next one directly below) */
                if(*state == FILTER_ST_START)
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter halt: Index %i: %s\n", i, name);
#endif
                    instData->filters[i]->f_stop(&(instData->filter_instance[i]));
                    *state = FILTER_ST_OPEN;
                }

                /* Go from open to DaAvStateAppActive */
                if(*state == FILTER_ST_OPEN)
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter close: Index %i: %s\n", i, name);
#endif
                    result = instData->filters[i]->f_close(&(instData->filter_instance[i]));
                    *state = FILTER_ST_IDLE;
                }
                break;

            case FILTER_START:
                /* Ignore same state */
                if(*state == FILTER_ST_START)
                {
                    stateok = TRUE;
                }

                /* Go from open to started */
                if(*state == FILTER_ST_OPEN)
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter start: Index %i: %s\n", i, name);
#endif
                    result = instData->filters[i]->f_start(&(instData->filter_instance[i]));
                    *state = FILTER_ST_START;
                }
                break;

            case FILTER_STOP:
                /* Ignore same state */
                if(*state == FILTER_ST_OPEN)
                {
                    stateok = TRUE;
                }

                /* Go from started to open */
                if(*state == FILTER_ST_START)
                {
                    stateok = TRUE;
#ifdef DEBUG
                    CL_API_LOGPrintf("Filter stop: Index %i: %s\n", i, name);
#endif
                    result = instData->filters[i]->f_stop(&(instData->filter_instance[i]));
                    *state = FILTER_ST_OPEN;
                }
                break;

            default:
                CL_API_LOGPrintf("Invalid opcode to filter helper\n");
                result = FALSE;
                stateok = TRUE;
                state = NULL;
                break;
        }
    }

    /* Make sure state points to something */
    if(state == NULL)
    {
        CsrUint8 tmp = 0xFF;
        state = &tmp;
    }

    /* Print error */
    if(!stateok)
    {
        CL_API_LOGPrintf("Filter transition error: Index %i, opcode %i, state %i: %s\n",
               i, op, *state, name);
    }
    if(!result)
    {
        CL_API_LOGPrintf("Filter function error: Index %i, opcode %i, state %i: %s\n",
               i, op, *state, name);
    }
    return result;
}

/**************************************************************************************************
 * filtersCheck
 **************************************************************************************************/
CsrBool filtersCheck(Av2AppInst *instData)
{
    int j;
    CsrUint8 mask;
    CsrBool fail;

    /* We havn't detected anything yet */
    mask = 0;
    fail = FALSE;

    j = 0;
    while((j<instData->filter_count) &&
          (instData->filters[j] != NULL) &&
          !fail)
    {
#ifdef DEBUG
        CL_API_LOGPrintf("Filter check: Index %i: %s : ",
               j, instData->filters[j]->f_name);
#endif

        /* Check for producer */
        if(instData->filters[j]->f_type & FILTER_PRODUCER)
        {
#ifdef DEBUG
            CL_API_LOGPrintf("producer ");
#endif
            if(mask & FILTER_PRODUCER)
            {
                fail = TRUE;
                break;
            }
            mask |= FILTER_PRODUCER;
        }

        /* Check for consumer */
        if(instData->filters[j]->f_type & FILTER_CONSUMER)
        {
#ifdef DEBUG
            CL_API_LOGPrintf("consumer ");
#endif
            if(mask & FILTER_CONSUMER)
            {
                fail = TRUE;
                break;
            }
            mask |= FILTER_CONSUMER;
        }

        /* Check for encoder */
        if(instData->filters[j]->f_type & FILTER_ENCODER)
        {
#ifdef DEBUG
            CL_API_LOGPrintf("encoder ");
#endif
            if(mask & FILTER_ENCODER)
            {
                fail = TRUE;
                break;
            }
            mask |= FILTER_ENCODER;
        }

        /* Check for decoder */
        if(instData->filters[j]->f_type & FILTER_DECODER)
        {
#ifdef DEBUG
            CL_API_LOGPrintf("decoder ");
#endif
            if(mask & FILTER_DECODER)
            {
                fail = TRUE;
                break;
            }
            mask |= FILTER_DECODER;
        }

        /* Check for pass-through (multiple of these allowed) */
        if(instData->filters[j]->f_type & FILTER_PASS)
        {
#ifdef DEBUG
            CL_API_LOGPrintf("pass-through ");
#endif
        }

#ifdef DEBUG
        CL_API_LOGPrintf("\n");
#endif
        j++;
    }

    if(fail)
    {
        CL_API_LOGPrintf("Multiple producers/consumers/encoders/decoders detected!\n");
    }

    return !fail;
}

/**************************************************************************************************
 * filtersQosUpdate
 **************************************************************************************************/
void filtersQosUpdate(Av2AppInst *instData,
                      CsrUint16 qosLevel)
{
    int i;

    i = 0;
    while(instData->filters[i] != NULL)
    {
        instData->filters[i]->f_qos_update(&(instData->filter_instance[i]),
                                           qosLevel);
        i++;
    }
}

/**************************************************************************************************
 * filtersGetConfig
 **************************************************************************************************/
CsrUint8 *filtersGetConfig(Av2AppInst *instData,
                          CsrUint8 *length)
{
    int i;

    i = 0;
    while(instData->filters[i] != NULL)
    {
        /* Check if the filter is a codec */
        if(instData->filters[i]->f_type & (FILTER_DECODER|FILTER_ENCODER))
        {
            return instData->filters[i]->f_get_config(&(instData->filter_instance[i]),
                                                      length);
        }
        i++;
    }

    CL_API_LOGPrintf("Did not find a codec filter that supports 'get configuration'\n");
    *length = 0;
    return NULL;
}

/**************************************************************************************************
 * filtersSetConfig
 **************************************************************************************************/
CsrBtAvResult filtersSetConfig(Av2AppInst *instData,
                             CsrUint8 *conf,
                             CsrUint8 conf_len)
{
    int i;

    i = 0;
    while(instData->filters[i] != NULL)
    {
        /* Check if the filter is a codec */
        if(instData->filters[i]->f_type & (FILTER_DECODER|FILTER_ENCODER))
        {
            return instData->filters[i]->f_set_config(&(instData->filter_instance[i]),
                                                      conf,
                                                      conf_len);

        }
        i++;
    }

    CL_API_LOGPrintf("Did not find a codec filter that supports 'set configuration'\n");
    return CSR_BT_RESULT_CODE_A2DP_INVALID_CODEC_TYPE;
}

/**************************************************************************************************
 * filtersGetCaps
 **************************************************************************************************/
CsrUint8 *filtersGetCaps(Av2AppInst *instData,
                        CsrUint8 *length)
{
    int i;

    i = 0;
    while(instData->filters[i] != NULL)
    {
        /* Check if the filter is a codec */
        if(instData->filters[i]->f_type & (FILTER_DECODER|FILTER_ENCODER))
        {
            return instData->filters[i]->f_get_caps(&(instData->filter_instance[i]),
                                                    length);
        }
        i++;
    }

    CL_API_LOGPrintf("Did not find a codec filter that supports 'get capabilities'\n");
    *length = 0;
    return NULL;
}

/**************************************************************************************************
 * filtersGetCaps
 **************************************************************************************************/
CsrUint8 *filtersRemoteCaps(Av2AppInst *instData,
                           CsrUint8 *caps,
                           CsrUint8 caps_len,
                           CsrUint8 *conf_len)
{
    int i;

    i = 0;
    while(instData->filters[i] != NULL)
    {
        /* Check if the filter is a codec */
        if(instData->filters[i]->f_type & (FILTER_DECODER|FILTER_ENCODER))
        {
            return instData->filters[i]->f_remote_caps(&(instData->filter_instance[i]),
                                                       caps,
                                                       caps_len,
                                                       conf_len);
        }
        i++;
    }

    CL_API_LOGPrintf("Did not find a codec filter that supports 'remote configuration'\n");
    *conf_len = 0;
    return NULL;
}
