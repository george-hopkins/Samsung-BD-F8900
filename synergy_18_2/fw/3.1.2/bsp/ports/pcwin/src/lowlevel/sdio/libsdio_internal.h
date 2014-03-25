#ifndef LIBSDIO_INTERNAL_H__
#define LIBSDIO_INTERNAL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
/* #include <winioctl.h> already included by windows.h */

#include "csr_types.h"

#include "uif.h"
#include "sdio.h"
#include "libsdio.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cmd_queue_elem
{
    struct cmd_queue_elem *next;
    sdio_io_callback_t     callback;
    void                  *arg;
    struct sdioemb_uif_cmd cmd;
    CsrUint8               write8_data;
};

struct cmd_queue
{
    struct cmd_queue_elem *head;
    struct cmd_queue_elem *tail;
};

struct sdio_uif
{
    sdio_int_handler_t int_handler;
    void              *arg;

    HANDLE handle;
    HANDLE int_thread;
    HANDLE int_thread_stop_event;
    HANDLE int_unmasked_event;

    struct sdioemb_uif_card_info card_info;
    struct sdioemb_uif_func_info func_info[SDIO_MAX_FUNCTIONS];

    struct cmd_queue cmd_queue;
    HANDLE           cmd_queue_sem;
    HANDLE           cmd_queue_stop_event;
    CRITICAL_SECTION cmd_queue_mutex;
    HANDLE           async_thread;
};

int uif_ioctl(sdio_uif_t uif, DWORD ioctl, struct sdioemb_uif_input *input,
    struct sdioemb_uif_output *output);

int async_thread_start(sdio_uif_t uif);
void async_thread_stop(sdio_uif_t uif);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef LIBSDIO_INTERNAL_H__ */
