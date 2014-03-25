#ifndef CSR_USR_CONFIG_DEFAULT_H__
#define CSR_USR_CONFIG_DEFAULT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_log_cleartext.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------
 * Defines for the application framework
 *--------------------------------------------------------------------------*/
#ifndef CSR_SCHEDULER_INSTANCES
#define CSR_SCHEDULER_INSTANCES 3
#endif

/*--------------------------------------------------------------------------
 * Defines for the generic scheduler
 *--------------------------------------------------------------------------*/

/*
 * The maximum number of messages to store in the
 * per-scheduler instance message container free list.
 * Helps reducing allocations in the message put path.
 */
#ifndef CSR_SCHED_MESSAGE_POOL_LIMIT
#define CSR_SCHED_MESSAGE_POOL_LIMIT    10
#endif

/*
 * The maximum number of timers to store in the
 * per-scheduler instance timer container free list.
 * Helps reducing allocations in the timer set path.
 */
#ifndef CSR_SCHED_TIMER_POOL_LIMIT
#define CSR_SCHED_TIMER_POOL_LIMIT      10
#endif

/*--------------------------------------------------------------------------
 * Defines for the BlueCore bootstrap procedure
 *--------------------------------------------------------------------------*/

/*
 * The fixed time (in us) to wait after a reset command, before the transport
 * is restarted.
 */
#ifndef CSR_BLUECORE_RESET_TIMER
#define CSR_BLUECORE_RESET_TIMER 500000
#endif

/*
 * Enable this option to enable an application to control the activation and
 * deactivation of the BlueCore.
 */
#ifndef CSR_BLUECORE_ONOFF
/*#define CSR_BLUECORE_ONOFF*/
#endif

/*
 * The maximum time (in us) to wait for the BlueCore to come alive after
 * sending a reset command. Only applicable when CSR_BLUECORE_ONOFF is defined.
 */
#ifndef CSR_BLUECORE_RESET_TIMEOUT
#define CSR_BLUECORE_RESET_TIMEOUT 5000000
#endif

/*
 * Enable this to periodically send a command to the BlueCore to monitor the
 * state of the communication link. If the BlueCore communication is lost, a
 * CSR_TM_BLUECORE_TRANSPORT_DEACTIVATE_IND will be sent to the application
 * that activated the BlueCore transport, requesting it to deactivate the
 * transport. Leave undefined to disable this functionality. Only applicable
 * when CSR_BLUECORE_ONOFF is defined.
 */
#ifndef CSR_BLUECORE_PING_INTERVAL
/*#define CSR_BLUECORE_PING_INTERVAL 5000000*/
#endif

/*
 * The maximum time (in us) to wait for the response to a BlueCore command.
 * If no response is received within this time limit, the communication link
 * will be considered lost, and a CSR_TM_BLUECORE_TRANSPORT_DEACTIVATE_IND
 * will be sent to the application that activated the BlueCore transport,
 * requesting it to deactivate the transport. Only applicable when
 * CSR_BLUECORE_ONOFF is defined.
 */
#ifndef CSR_BCCMD_CMD_TIMEOUT
#define CSR_BCCMD_CMD_TIMEOUT 2000000
#endif

/*--------------------------------------------------------------------------
 * Defines for Type-A
 *--------------------------------------------------------------------------*/

/*
 * Type-A deep sleep enable timeout in ms.
 * If 0, deep sleep is disabled.
 */
#ifndef CSR_TYPE_A_SLEEP_TIMEOUT
#define CSR_TYPE_A_SLEEP_TIMEOUT    (0)
#endif

/*
 * The maximum bus speed to set during
 * normal operation.
 */
#ifndef CSR_TYPE_A_BUSSPEED_AWAKE
#define CSR_TYPE_A_BUSSPEED_AWAKE     (12500000)
#endif

/*
 * Type-A deep sleep wakeup delay in ms.
 */
#ifndef CSR_TYPE_A_WAKEUP_TIMEOUT
#define CSR_TYPE_A_WAKEUP_TIMEOUT   (125)
#endif

/* Type-A initialisation delay in ms */
#ifndef CSR_TYPE_A_ENABLE_DELAY
#define CSR_TYPE_A_ENABLE_DELAY     (1000)
#endif

/* Time to wait for chip wakeup in ms */
#ifndef CSR_TYPE_A_WAKEUP_DELAY
#define CSR_TYPE_A_WAKEUP_DELAY     (1000)
#endif

#ifndef CSR_TYPE_A_RXBUF_POOLSIZE
#define CSR_TYPE_A_RXBUF_POOLSIZE   (32)
#endif

#ifndef CSR_TYPE_A_RXBUF_ELMSIZE
#define CSR_TYPE_A_RXBUF_ELMSIZE    (8 * 1024)
#endif

#ifndef CSR_TYPE_A_TXBUF_SIZE
/* Maximum Type-A buffer size. */
#define CSR_TYPE_A_TXBUF_SIZE       (64 * 1024 + 4 + 4)
#endif

/*--------------------------------------------------------------------------
 * Defines for BCSP
 *--------------------------------------------------------------------------*/

/* If CSR_ABCSP_TXCRC is #defined then the optional CRC field is appended to each
BCSP message transmitted, else the CRC is not appended. Do NOT define in this
file, must be a global definition, i.e. give it as argument to the compiler */

#define CSR_ABCSP_TXCRC

/* The CRC field is optional on BCSP messages.  If CSR_ABCSP_RXCRC is #defined
then the CRC fields found on all received BCSP messages are checked.  If
CSR_ABCSP_RXCRC is not #defined then no received CRC field is checked; this means
that packets with invalid CRCs may be accepted as good messages. Please also note
that if this is undefined it should also be undefined on the BC which will send
two unnecessary CRC bytes on every reliable BCSP packet to the host */

#define CSR_ABCSP_RXCRC


/*
 * BCSP retransmission timer and BCSP timer jitter.
 *
 * The BCSP retransmission timer is used to control for how
 * long the host waits for a response from the chip before
 * retransmitting a message.
 * If CSR_BCSP_RETRANSMISSION_TIMER is defined, its value is
 * used as the BCSP retransmission timer period value.
 * If not, the value is derived from the currently configured
 * (i.e. at runtime) UART speed.
 * By default, a 250ms timer is used.
 *
 * The retransmission timer is jittered to avoid scenarios
 * where the chip may keep missing the retransmission due to
 * unfortunate timing.  The jitter value controls the limits for
 * the random offset applied to the base retransmission timer value.
 * A value of 10ms is used by default which together with a 250ms
 * baseline of retransmission timer base gives a retransmission
 * timer in the interval [240ms; 260ms].
 *
 * CSR_BCSP_RETRANSMISSION_MINIMUM specifies the minimum period
 * used when jittering is enabled.
 */

/*
 */
#ifndef CSR_BCSP_AUTO_TIMER
#ifndef CSR_BCSP_RETRANSMISSION_TIMER
#define CSR_BCSP_RETRANSMISSION_TIMER                  (250000)
/* 0.25 second */
#endif /* CSR_BCSP_RETRANSMISSION_TIMER */
#endif

#ifndef CSR_BCSP_RETRANSMISSION_JITTER
#define CSR_BCSP_RETRANSMISSION_JITTER                 (10000)
/* +/- 10ms */
#endif /* CSR_BCSP_RETRANSMISSION_JITTER */

#ifndef CSR_BCSP_RETRANSMISSION_MINIMUM
#define CSR_BCSP_RETRANSMISSION_MINIMUM                (10000)
/* 10ms */
#endif

#ifndef CSR_BCSP_TSHY_TIMER
#define CSR_BCSP_TSHY_TIMER                            (1000000)
/* 1 second */
#endif /* CSR_BCSP_TSHY_TIMER */

#ifndef CSR_BCSP_TCONF_TIMER
#define CSR_BCSP_TCONF_TIMER                           (1000000)
/* 1 second */
#endif /* CSR_BCSP_TCONF_TIMER */


/* The size of the BCSP transmit window.  This must be between 1 and 7.  This
is normally set to 4.  This is called "winsiz" in the BCSP protocol
specification.

This determines the number of BCSP messages that can be handled by the abcsp
library's transmit path at a time, so it affects the storage requirements for
ABCSP_TXMSG messages. */

#ifndef CSR_ABCSP_TXWINSIZE
#define CSR_ABCSP_TXWINSIZE                                         (4)
#endif /* CSR_ABCSP_TXWINSIZE */

/*--------------------------------------------------------------------------
 * Defines for FastPipe
 *--------------------------------------------------------------------------*/

#define CSR_FP_CONTROLER_CREDIT_MAX       (3132)
#define CSR_FP_CONTROLLER_PACKET_SIZE_MAX (1048)
#define CSR_FP_HOST_PACKET_SIZE_MAX       (1)
#define CSR_FP_PACEKTS_MAX                (7)


/*--------------------------------------------------------------------------
 * Defines for Csr Log
 *--------------------------------------------------------------------------*/
/* Defines the maximum string length that will be written to the log transport
 * in one call to the CSR_LOG_TEXT_XXX() functions found in csr_log_text.h.
 * NB: This limit does not apply to the CSR_LOG_TEXT() macro. */
#define CSR_LOG_TEXT_MAX_STRING_LEN 255

/* Defines an upper limit in bytes on the amount of primitive data to write in
 * a put/get/pop/save message entry. NB: This limit only applies if the log
 * level define CSR_LOG_LEVEL_TASK_PRIM_APPLY_LIMIT is set for a task.
 *
 * WARNING: Using this will seriusly affect the readability of the wireshark
 * logs, but it might help as a measure to reduce the amount of log info
 * generated on a platform */
#define CSR_LOG_PRIM_SIZE_UPPER_LIMIT 64

/* Specify the output template format for the cleartext logger
 * (see csr_log_cleartext.h) for a description of all possible templates */
#define CSR_LOG_CLEARTEXT_FORMAT CSR_LOG_CLEARTEXT_TEMPLATE_YEAR "/" CSR_LOG_CLEARTEXT_TEMPLATE_MONTH "/" CSR_LOG_CLEARTEXT_TEMPLATE_DAY " " CSR_LOG_CLEARTEXT_TEMPLATE_HOUR ":" CSR_LOG_CLEARTEXT_TEMPLATE_MIN ":" CSR_LOG_CLEARTEXT_TEMPLATE_TIME_SEC ":" CSR_LOG_CLEARTEXT_TEMPLATE_TIME_MSEC " " CSR_LOG_CLEARTEXT_TEMPLATE_TASK_NAME " " CSR_LOG_CLEARTEXT_TEMPLATE_SUBORIGIN_NAME " " CSR_LOG_CLEARTEXT_TEMPLATE_LOG_LEVEL_NAME ": " CSR_LOG_CLEARTEXT_TEMPLATE_STRING CSR_LOG_CLEARTEXT_TEMPLATE_BUFFER

/*--------------------------------------------------------------------------
 * Defines for the IP stack
 *--------------------------------------------------------------------------*/
/* By default the IP stack assumes that the host architecture is little endian
 * if this is not the case the define below should be defined to force the IP
 * stack to use big endian */
/* #define CSR_IP_USE_BIG_ENDIAN */

/*--------------------------------------------------------------------------
 * Defines for the CSR_DATA_STORE
 *--------------------------------------------------------------------------*/
/* This define specifies the directory where the CSR_DATA_STORE task places
 * its database files current default is in the directory "data_store" relative
 * to where the executable is started.
 * NB: It is important that this path ends with a trailing '/' */
#define CSR_DATA_STORE_ROOT_DIR "./data_store/"

#define CSR_DATA_STORE_DEFAULT_ENTRY_NAME "ds_info.cdi"

/*--------------------------------------------------------------------------
 * Defines for Chip Manager
 *--------------------------------------------------------------------------*/
/* Default number of microseconds between sending PING request */
#ifndef CSR_BLUECORE_DEFAULT_PING_INTERVAL
#define CSR_BLUECORE_DEFAULT_PING_INTERVAL (5000000)
/* 5 sec */
#endif

/*--------------------------------------------------------------------------
 * Defines for DSPM
 *--------------------------------------------------------------------------*/
/* If support for downloading capabilities in DSPM is not required, this
   definition can be removed to reduce the code size. */
#define CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD

/*--------------------------------------------------------------------------
 * Defines for pclin DHCP client configuration
 *--------------------------------------------------------------------------*/
/* Full path to dhcpcd (used on Android) and dhclient. */
#ifndef DHCPCD_PATH
#define DHCPCD_PATH     "/sbin/dhcpcd"
#endif

#ifndef DHCLIENT_PATH
#define DHCLIENT_PATH   "/sbin/dhclient"
#endif

/*--------------------------------------------------------------------------
 * Defines for DHCP_SERVER
 *--------------------------------------------------------------------------*/
/*
 * Uncomment this to enable unicasting of DHCP messages:
 *
 *  #define CSR_DHCP_SERVER_USE_IFCONFIG_ARP
 */

#ifdef __cplusplus
}
#endif

#endif
