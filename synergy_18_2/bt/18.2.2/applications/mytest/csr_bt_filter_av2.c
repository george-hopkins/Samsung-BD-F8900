/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_av_lib.h"
#include "csr_bt_av_prim.h"
#include "Bluetooth_AV.h"
#include "Bluetooth_Common.h"


/* #define DEBUG */
#undef DEBUG
/* Filter entry prototypes */
static CsrBool av2_init(void **instance, struct av2filter_t *filter, void *unused);
static CsrBool av2_deinit(void **instance);
static CsrBool av2_open(void **instance);
static CsrBool av2_close(void **instance);
static CsrBool av2_start(void **instance);
static CsrBool av2_stop(void **instance);
static CsrBool av2_process_source(CsrUint8 index,
                                 av2filter_t **chain,
                                 CsrBool freeData,
                                 void *data,
                                 CsrUint32 length);
static CsrBool av2_process_sink(CsrUint8 index,
                               av2filter_t **chain,
                               CsrBool freeData,
                               void *data,
                               CsrUint32 length);
static CsrUint8 *av2_get_config(void **instance,
                                    CsrUint8 *conf_len);
static CsrBtAvResult av2_set_config(void **instance,
                                       CsrUint8 *conf,
                                       CsrUint8 conf_len);
static CsrUint8 *av2_get_caps(void **instance,
                                  CsrUint8 *cap_len);
static CsrUint8 *av2_remote_caps(void **instance,
                                     CsrUint8 *caps,
                                     CsrUint8 caps_len,
                                     CsrUint8 *conf_len);
static void av2_qos_update(void **instance,
                           CsrUint16 qosLevel);
static void av2_menu(void **instance,
                          CsrUint8 *num_options);
static CsrBool av2_setup(void **instance,
                             CsrUint8 index,
                             char *value);

/* Filter structure setup for the AV2 "last source" */
av2filter_t av2_source_filter =
{
    "csr_source_transmitter",
    FILTER_PASS,
    AV_NO_MEDIA,
    AV_NO_CODEC,
    0,
    NULL,
    av2_init,
    av2_deinit,
    av2_open,
    av2_close,
    av2_start,
    av2_stop,
    av2_process_source,
    av2_get_config,
    av2_set_config,
    av2_get_caps,
    av2_remote_caps,
    av2_qos_update,
    av2_menu,
    av2_setup
};

/* Filter structure setup for the AV2 "last sink" */
av2filter_t av2_sink_filter =
{
    "csr_sink_terminator",
    FILTER_PASS,
    AV_NO_MEDIA,
    AV_NO_CODEC,
    0,
    NULL,
    av2_init,
    av2_deinit,
    av2_open,
    av2_close,
    av2_start,
    av2_stop,
    av2_process_sink,
    av2_get_config,
    av2_set_config,
    av2_get_caps,
    av2_remote_caps,
    av2_qos_update,
    av2_menu,
    av2_setup
};

/* Source processer -- transmit data! */
static CsrBool av2_process_source(CsrUint8 index,
                                 av2filter_t **chain,
                                 CsrBool freeData,
                                 void *data,
                                 CsrUint32 length)
{
    extern Av2AppInst *Av2Instance;
    CsrBool ate;

    ate = FALSE;

    /* Perform the transmission. If we are streaming two sources, we copy the payload. This
     * greatly optimises the send-process as we don't have to run the encode-process again.
 */
    if(Av2Instance->state == SsAvStateAvConnectedStreaming)
    {
        ate = TRUE;
        CsrBtAvStreamDataReqSend(Av2Instance->streamHandle,
                            FALSE,
                            FALSE,
                            Av2Instance->mediaPayloadType,
                            Av2Instance->time_stamp,
                            (CsrUint16)length,
                            data);
    }

    /* Silently consume data if required */
    if(freeData && data && !ate)
    {
        CsrPmemFree(data);
    }
    return ate;
}

/* Sink terminator -- free data if possible */
static CsrBool av2_process_sink(CsrUint8 index,
                               av2filter_t **chain,
                               CsrBool freeData,
                               void *data,
                               CsrUint32 length)
{
    /* Simply check that data has been consumed */
#ifdef DEBUG
    if(data)
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,"Last sink filter received data - this should not happen (filter index %i)\n",
               index);
    }
#endif
    if(freeData && data)
    {
        CsrPmemFree(data);
    }
    return (data == NULL);
}

/* Initialise av2 filter */
static CsrBool av2_init(void **instance, struct av2filter_t *filter, void *unused)
{
    /* Not required */
    *instance = NULL;
    return TRUE;
}

/* Deinitialise av2 filter */
static CsrBool av2_deinit(void **instance)
{
    /* Not required */
    return TRUE;
}

/* Open av2 with current configuration */
static CsrBool av2_open(void **instance)
{
    /* Not required */
    return TRUE;
}

/* Close av2 configuration */
static CsrBool av2_close(void **instance)
{
    /* Not required */
    return TRUE;
}

/* Start av2 streaming */
static CsrBool av2_start(void **instance)
{
    /* Not required */
    return TRUE;
}

/* Stop av2 streaming */
static CsrBool av2_stop(void **instance)
{
    /* Not required */
    return TRUE;
}

/* Return av2 configuration */
static CsrUint8 *av2_get_config(void **instance,
                                   CsrUint8 *conf_len)
{
    /* We do not support this feature */
    *conf_len = 0;
    return NULL;
}

/* Set av2 configuration */
static CsrBtAvResult av2_set_config(void **instance,
                                      CsrUint8 *conf,
                                      CsrUint8 conf_len)
{
    /* We do not support this feature */
    return CSR_BT_AV_ACCEPT;
}

/* Return av2 service capabilities */
static CsrUint8 *av2_get_caps(void **instance,
                                 CsrUint8 *cap_len)
{
    /* We do not support this feature */
    *cap_len = 0;
    return NULL;
}

/* Investigate remote capabilities, return optimal configuration */
static CsrUint8 *av2_remote_caps(void **instance,
                                    CsrUint8 *caps,
                                    CsrUint8 caps_len,
                                    CsrUint8 *conf_len)
{
    /* We do not support this feature */
    *conf_len = 0;
    return NULL;
}

static void av2_qos_update(void **instance,
                           CsrUint16 qosLevel)
{
    /* Not supported */
}

/* Print menu options to screen */
static void av2_menu(void **instance,
                         CsrUint8 *num_options)
{
    /* Not supported */
  //  CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,"  AV2 filter terminators does not support menu options\n");
    *num_options = 0;
}

/* Set user option */
static CsrBool av2_setup(void **instance,
                            CsrUint8 index,
                            char *value)
{
    CsrPmemFree(value);
    return FALSE;
}
