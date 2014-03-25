#ifndef _USR_CONFIG_H
#define _USR_CONFIG_H
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           usr_config.h

DESCRIPTION:    This file contains settings that may be changed at compile time
                for different defines used throughout the CPL components.
                A number of the defines are assigned default values if not otherwise
                set from the compile environment.

REVISION:       $Revision: #16 $

****************************************************************************/

#include "bchs_tasks.h"
#include "sched/csr_types.h"
#include "bluetooth.h"
#include "hci.h"
#include "dm_prim.h"
#include "rfcomm_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bluetooth versions as defined by the specification (DO NOT MODIFY) */
#ifndef BLUETOOTH_VERSION_1P0
#define BLUETOOTH_VERSION_1P0                       (0x00)
#endif
#ifndef BLUETOOTH_VERSION_1P1
#define BLUETOOTH_VERSION_1P1                       (0x01)
#endif
#ifndef BLUETOOTH_VERSION_1P2
#define BLUETOOTH_VERSION_1P2                       (0x02)
#endif
#ifndef BLUETOOTH_VERSION_2P0
#define BLUETOOTH_VERSION_2P0                       (0x03)
#endif
#ifndef BLUETOOTH_VERSION_2P1
#define BLUETOOTH_VERSION_2P1                       (0x04)
#endif

/*--------------------------------------------------------------------------
 * General defines for BCHS
 *--------------------------------------------------------------------------*/

/* This define specifies the Bluetooth version that should be supported by
 * BCHS. Setting BCHS_BT_VERSION lower than default will exclude features
 * requiring a newer Bluetooth version.
 * For example will BCHS_BT_VERSION == BLUETOOTH_VERSION_2P0
 * exclude support for Secure Simple Pairing, Extended Inquiry Response and
 * other features only supported by Bluetooth versions newer than 2.0. The
 * benefit of decreasing BCHS_BT_VERSION is that the code base and memory
 * consumption of BCHS can be reduced. Possible values for the define can be
 * found in profiles.h */
#ifndef BCHS_BT_VERSION
#define BCHS_BT_VERSION                         (BLUETOOTH_VERSION_2P1)
#endif /* BCHS_BT_VERSION */

/*--------------------------------------------------------------------------
 * Defines for Service Discovery (SD) module
 *--------------------------------------------------------------------------*/

/* The maximum number of search results to send to the application during a
 * device search */
#ifndef SD_SEARCH_MAX_RESULTS_DEFAULT
#define SD_SEARCH_MAX_RESULTS_DEFAULT           (BCHS_UNLIMITED)
#endif /* SD_SEARCH_MAX_RESULTS_DEFAULT */

/* Time-out in milliseconds for reading the name of a remote device */
#ifndef SD_READ_NAME_TIMEOUT_DEFAULT
#define SD_READ_NAME_TIMEOUT_DEFAULT            (5000)
#endif /* SD_READ_NAME_TIMEOUT_DEFAULT */

/* Bitmask specifying the default configuration to be used for device search */
#ifndef SD_SEARCH_CONFIG_DEFAULT
#define SD_SEARCH_CONFIG_DEFAULT                (SD_SEARCH_USE_STANDARD)
#endif /* SD_SEARCH_CONFIG_DEFAULT */

/* Bitmask specifying the default memory configuration for the SD - must be
 * set to SD_MEMORY_CONFIG_USE_STANDARD for now */
#ifndef SD_MEMORY_CONFIG_DEFAULT
#define SD_MEMORY_CONFIG_DEFAULT                (SD_MEMORY_CONFIG_USE_STANDARD)
#endif /* SD_MEMORY_CONFIG_DEFAULT */

/* Max total number of devices to store in the Device List - must be set to
 * BCHS_UNLIMITED */
#ifndef SD_MEMORY_LIST_MAX_DEFAULT
#define SD_MEMORY_LIST_MAX_DEFAULT              (BCHS_UNLIMITED)
#endif /* SD_MEMORY_LIST_MAX_DEFAULT */

/* Max number of devices to store extended information (name, services...)
   for. Must be smaller or equal to SD_MEMORY_LIST_MAX_DEFAULT and must be
   set to a minimum of 1 */
#ifndef SD_MEMORY_LIST_INFO_MAX_DEFAULT
#define SD_MEMORY_LIST_INFO_MAX_DEFAULT         (25)
#endif /* SD_MEMORY_LIST_INFO_MAX_DEFAULT */

/*--------------------------------------------------------------------------
 * Defines for Class Of Device (COD) and service/friendly names
 *--------------------------------------------------------------------------*/
/* Use to define the Major and Minor device class fields of the Class
 * of Device. */
#ifndef MAJOR_MINOR_DEVICE_CLASS
#define MAJOR_MINOR_DEVICE_CLASS               0x000208 /* (PHONE_MAJOR_DEVICE_MASK + CORDLESS_MINOR_DEVICE_MASK) */
#endif /* MAJOR_MINOR_DEVICE_CLASS */

/* Max name length of the service name for a registered service */
#ifndef MAX_SERVICE_NAME_LEN
#define MAX_SERVICE_NAME_LEN                                    (50)
#endif /* MAX_SERVICE_NAME_LEN */

/* This is the maximum amount of chars to be passed between the
 * profile layer and the application for the remote name if the name
 * is shorter than the max length, the name must be zero terminated.
 * If a name longer than 40 chars (20 words) is sent to the chip during
 * bootstrap, the name will be ignored. Max length for the friendly name
 * is defined as 248 octets in HCI, but this will be limited in BCHS
 * by this define.
 */
#ifndef MAX_FRIENDLY_NAME_LEN
#define MAX_FRIENDLY_NAME_LEN                                   (40)
#endif /* MAX_FRIENDLY_NAME_LEN */

/* Define the default startup connectable mode. */
#ifndef CM_DEFAULT_STARTUP_CONNECTABLE_MODE
#define CM_DEFAULT_STARTUP_CONNECTABLE_MODE                     (HCI_SCAN_ENABLE_INQ)
#endif /* CM_DEFAULT_STARTUP_CONNECTABLE_MODE */

/*--------------------------------------------------------------------------
  Defines for BCHS L2CAP connection
  ----------------------------------------------------------------------------*/
/* Define the number of simultaneously L2CAP connections, that can be
 * used at the same time. */
#ifndef MAX_NUM_OF_SIMULTANIOUS_L2CAP_CONNECTIONS
#define MAX_NUM_OF_SIMULTANIOUS_L2CAP_CONNECTIONS               (6)
#endif /* MAX_NUM_OF_SIMULTANIOUS_L2CAP_CONNECTIONS */

/* For L2CAP the frame size must be >= 48 bytes. It is recommended,
 * due to speed optimization, that the frame  size is set to 672 */
#ifndef PROFILE_L2CA_DEFAULT_MAX_FRAME_SIZE
#define PROFILE_L2CA_DEFAULT_MAX_FRAME_SIZE                     (672)
#endif /* PROFILE_L2CA_DEFAULT_MAX_FRAME_SIZE */

/*--------------------------------------------------------------------------
  Defines for BCHS RFCOMM connection
  ----------------------------------------------------------------------------*/

/* Define the number of simultaneously RFCOMM connections, that can be
 * used at the same time. */
#ifndef MAX_NUMBER_OF_SIMULTANIOUS_CONNECTIONS
#define MAX_NUMBER_OF_SIMULTANIOUS_CONNECTIONS                  (4)
#endif /* MAX_NUMBER_OF_SIMULTANIOUS_CONNECTIONS */

/* For the RFCOMM build the frame size must be within 23 - 720.
 * It is recommended, due to speed optimization, that the frame
 * size is set to 330 unless RFC_BUILD is not defined */
#ifndef RFC_BUILD
#ifdef EDR_DISABLE
#ifndef PROFILE_DEFAULT_MAX_FRAME_SIZE
#define PROFILE_DEFAULT_MAX_FRAME_SIZE                          (330)
#endif /* PROFILE_DEFAULT_MAX_FRAME_SIZE */
#else
#ifndef PROFILE_DEFAULT_MAX_FRAME_SIZE
#define PROFILE_DEFAULT_MAX_FRAME_SIZE                          (1012)
#endif /* PROFILE_DEFAULT_MAX_FRAME_SIZE */
#endif /* EDR_DISABLE */
#else
#ifndef PROFILE_DEFAULT_MAX_FRAME_SIZE
#define PROFILE_DEFAULT_MAX_FRAME_SIZE                          (330)
#endif /* PROFILE_DEFAULT_MAX_FRAME_SIZE */
#endif /* RFC_BUILD */

/* Defines how many data frames each RFCOMM connection in the CM is
 * able to accept before its buffer is fill up. The value between 1-255.
 * Setting this value low can result in a lower bandrate */
#ifndef CM_INIT_CREDIT
#define CM_INIT_CREDIT                                          (25)
#endif /* CM_INIT_CREDIT */

/* Port negotiation defines */
#ifndef DEFAULT_PORT_SPEED
#define DEFAULT_PORT_SPEED                                      (PORT_SPEED_115200)
#endif /* DEFAULT_PORT_SPEED */

#ifndef DEFAULT_DATA_BITS
#define DEFAULT_DATA_BITS                                       (DATA_BITS_8)
#endif /* DEFAULT_DATA_BITS */

#ifndef DEFAULT_STOP_BITS
#define DEFAULT_STOP_BITS                                       (STOP_BITS_ONE)
#endif /* DEFAULT_STOP_BITS */

#ifndef DEFAULT_PARITY
#define DEFAULT_PARITY                                          (PARITY_ON)
#endif /* DEFAULT_PARITY */

#ifndef DEFAULT_PARITY_TYPE
#define DEFAULT_PARITY_TYPE                                     (PARITY_TYPE_EVEN)
#endif /* DEFAULT_PARITY_TYPE */

#ifndef DEFAULT_FLOW_CTRL_MASK
#define DEFAULT_FLOW_CTRL_MASK                                  (FLC_RTR_INPUT)
#endif /* DEFAULT_FLOW_CTRL_MASK */

#ifndef DEFAULT_XON
#define DEFAULT_XON                                             (XON_CHAR_DEFAULT)
#endif /* DEFAULT_XON */

#ifndef DEFAULT_XOFF
#define DEFAULT_XOFF                                            (XOFF_CHAR_DEFAULT)
#endif /* DEFAULT_XOFF */

#ifndef DEFAULT_PARAMETER_MASK
#define DEFAULT_PARAMETER_MASK                                  (0X3FFF)
#endif /* DEFAULT_PARAMETER_MASK */


/*--------------------------------------------------------------------------
 * Defines for BCHS page scan
 *--------------------------------------------------------------------------*/
/* Define the Page Scan interval: 0x12 - 0x1000, default 0x800 */
#ifndef PAGE_SCAN_INTERVAL_DEFAULT
#define PAGE_SCAN_INTERVAL_DEFAULT                              (0x800)
#endif /* PAGE_SCAN_INTERVAL_DEFAULT */

/* Define the Page Scan window: 0x12 - 0x1000, default 0x12 */
#ifndef PAGE_SCAN_WINDOW_DEFAULT
#define PAGE_SCAN_WINDOW_DEFAULT                                (0x12)
#endif /* PAGE_SCAN_WINDOW_DEFAULT */

/* Define the timeout window before page request fails.
 * Sec = PAGE_TIMEOUT_DEFAULT * 0.000625 */
#ifndef PAGE_TIMEOUT_DEFAULT
#define PAGE_TIMEOUT_DEFAULT                                    (24000)
#endif /* PAGE_TIMEOUT_DEFAULT */

/*--------------------------------------------------------------------------
 * Defines for BCHS Page Scan Type and Inquiry Scan Type
 *--------------------------------------------------------------------------*/
/* Use HCI_SCAN_TYPE_LEGACY for standard scan.
 * HCI_SCAN_TYPE_INTERLACED for interlaced scan.
 *
 * Please note that the drawbacks of using HCI_SCAN_TYPE_INTERLACED
 * scan is that it uses more power than HCI_SCAN_TYPE_LEGACY. The
 * advantage of using HCI_SCAN_TYPE_INTERLACED is that a peer device
 * faster can make inquiries and connect to this
 * device. */
#ifndef PAGE_SCAN_TYPE_DEFAULT
#define PAGE_SCAN_TYPE_DEFAULT                                  (HCI_SCAN_TYPE_LEGACY)
#endif /* PAGE_SCAN_TYPE_DEFAULT */

#ifndef INQUIRY_SCAN_TYPE_DEFAULT
#define INQUIRY_SCAN_TYPE_DEFAULT                               (HCI_SCAN_TYPE_LEGACY)
#endif /* INQUIRY_SCAN_TYPE_DEFAULT */

/*--------------------------------------------------------------------------
  Defines for BCHS voice parameter
  ----------------------------------------------------------------------------*/
/* Voice setting parameter controls all the various settings for voice
 * connection. These settings apply to all voice connection, and
 * CANNOT be set for individual voice connections. The voice setting
 * parameter controls the configuration for voice connections : Input
 * coding, input data format, input sample size and air coding.
 *
 * In order to set the voice setting parameters, the user need to pick
 * one of each configuration parameter and OR them together. The
 * result must is the value of VOICE_SETTING.
 *
 * E.g VOICE_SETTING set to : *
 *   INPUTCODING_LINEAR | INPUTDATAFORMAT_2COMPLEMENT |
 *   INPUTSAMPLESIZE_16BIT | AIRCODING_CVSD)
 * means that the input coding is linear, the input data format is 2's
 * complement, the input sample size is 16 bit, and the aircoding is
 * CVSD */

/* Defines Input Coding */
#ifndef INPUTCODING_LINEAR
#define INPUTCODING_LINEAR                                      0x0000
#endif /* INPUTCODING_LINEAR */

#ifndef INPUTCODING_my_LAW
#define INPUTCODING_my_LAW                                      0x0100
#endif /* INPUTCODING_my_LAW */

#ifndef INPUTCODING_A_LAW
#define INPUTCODING_A_LAW                                       0x0200
#endif /* INPUTCODING_A_LAW */

/* Defines input data format */
#ifndef INPUTDATAFORMAT_1COMPLEMENT
#define INPUTDATAFORMAT_1COMPLEMENT                             0x0000
#endif /* INPUTDATAFORMAT_1COMPLEMENT */

#ifndef INPUTDATAFORMAT_2COMPLEMENT
#define INPUTDATAFORMAT_2COMPLEMENT                             0x0040
#endif /* INPUTDATAFORMAT_2COMPLEMENT */

#ifndef INPUTDATAFORMAT_SIGNMAGNITUDE
#define INPUTDATAFORMAT_SIGNMAGNITUDE                           0x0080
#endif /* INPUTDATAFORMAT_SIGNMAGNITUDE */

/* Defines input sample size */
#ifndef INPUTSAMPLESIZE_8BIT
#define INPUTSAMPLESIZE_8BIT                                    0x0000
#endif /* INPUTSAMPLESIZE_8BIT */

#ifndef INPUTSAMPLESIZE_16BIT
#define INPUTSAMPLESIZE_16BIT                                   0x0020
#endif /* INPUTSAMPLESIZE_16BIT */

/* Defines air Coding */
#ifndef AIRCODING_CVSD
#define AIRCODING_CVSD                                          0x0000
#endif /* AIRCODING_CVSD */

#ifndef AIRCODING_my_LAW
#define AIRCODING_my_LAW                                        0x0001
#endif /* AIRCODING_my_LAW */

#ifndef AIRCODING_A_LAW
#define AIRCODING_A_LAW                                         0x0002
#endif /* AIRCODING_A_LAW */

#ifndef AIRCODING_TRANSPARENT_DATA
#define AIRCODING_TRANSPARENT_DATA                              0x0003
#endif /* AIRCODING_TRANSPARENT_DATA */

/* This is the default air-coding voice setting */
#ifndef VOICE_SETTING_AIR
#define VOICE_SETTING_AIR                                       (AIRCODING_CVSD)
#endif /* VOICE_SETTING_AIR */

/* This is the default input-coding voice setting */
#ifndef VOICE_SETTING_INPUT
#define VOICE_SETTING_INPUT                                     (INPUTCODING_LINEAR | \
                                                                 INPUTDATAFORMAT_2COMPLEMENT | \
                                                                 INPUTSAMPLESIZE_16BIT )
#endif /* VOICE_SETTING_INPUT */

/* The default combined voice-setting. This will be written to the
 * chip when BCHS boots. Settings can after that only be changed if
 * the specific profile allows it, and only if there are no active SCO
 * connections. */
#ifndef VOICE_SETTING
#define VOICE_SETTING                                           (VOICE_SETTING_AIR | VOICE_SETTING_INPUT)
#endif /* VOICE_SETTING */


/*--------------------------------------------------------------------------
 * Defines for BCHS SCO/SYNC connection
 *--------------------------------------------------------------------------*/

/* Define the max number of simultaneously sync connections */
#ifndef MAX_NUM_OF_SIMULTANIOUS_SCO_CONNECTIONS
#define MAX_NUM_OF_SIMULTANIOUS_SCO_CONNECTIONS                (3)
#endif /* MAX_NUM_OF_SIMULTANIOUS_SCO_CONNECTIONS */

/* Default values for the accept_connect_sco for Handsfree
 * Gateway. Note that CVSD encoding is the only air-coding allowed for
 * HFG, so we use that and OR the default voice setting input
 * coding */
#ifndef SCO_DEFAULT_ACCEPT_TX_BANDWIDTH
#define SCO_DEFAULT_ACCEPT_TX_BANDWIDTH                         (0xFFFFFFFF)
#endif /* SCO_DEFAULT_ACCEPT_TX_BANDWIDTH */

#ifndef SCO_DEFAULT_ACCEPT_RX_BANDWIDTH
#define SCO_DEFAULT_ACCEPT_RX_BANDWIDTH                         (0xFFFFFFFF)
#endif /* SCO_DEFAULT_ACCEPT_RX_BANDWIDTH */

#ifndef SCO_DEFAULT_ACCEPT_MAX_LATENCY
#define SCO_DEFAULT_ACCEPT_MAX_LATENCY                          (0xFFFF)
#endif /* SCO_DEFAULT_ACCEPT_MAX_LATENCY */

#ifndef SCO_DEFAULT_ACCEPT_VOICE_SETTINGS
#define SCO_DEFAULT_ACCEPT_VOICE_SETTINGS                       (AIRCODING_CVSD | VOICE_SETTING_INPUT)
#endif /* SCO_DEFAULT_ACCEPT_VOICE_SETTINGS */

#ifndef SCO_DEFAULT_ACCEPT_RE_TX_EFFORT
#define SCO_DEFAULT_ACCEPT_RE_TX_EFFORT                         (0xFF)
#endif /* SCO_DEFAULT_ACCEPT_RE_TX_EFFORT */

/* 3M EDR eSCO packets cannot be used on BC4 with CVSD.  This value
 * can be changed to 0xFC3F if using BC5 or newer, or if another
 * aircoding is used. */
#ifndef SCO_DEFAULT_ACCEPT_AUDIO_QUALITY
#define SCO_DEFAULT_ACCEPT_AUDIO_QUALITY (0xFEBF)
#endif /* SCO_DEFAULT_ACCEPT_AUDIO_QUALITY */

#ifndef ESCO_DEFAULT_CONNECT_TX_BANDWIDTH
#define ESCO_DEFAULT_CONNECT_TX_BANDWIDTH                       (8000)
#endif /* ESCO_DEFAULT_CONNECT_TX_BANDWIDTH */

#ifndef ESCO_DEFAULT_CONNECT_RX_BANDWIDTH
#define ESCO_DEFAULT_CONNECT_RX_BANDWIDTH                       (8000)
#endif /* ESCO_DEFAULT_CONNECT_RX_BANDWIDTH */

#ifndef ESCO_DEFAULT_CONNECT_MAX_LATENCY
#define ESCO_DEFAULT_CONNECT_MAX_LATENCY                        (0x000A)
#endif /* ESCO_DEFAULT_CONNECT_MAX_LATENCY */

#ifndef ESCO_DEFAULT_CONNECT_VOICE_SETTINGS
#define ESCO_DEFAULT_CONNECT_VOICE_SETTINGS                     (AIRCODING_CVSD | VOICE_SETTING_INPUT)
#endif /* ESCO_DEFAULT_CONNECT_VOICE_SETTINGS */

#ifndef ESCO_DEFAULT_CONNECT_RE_TX_EFFORT
#define ESCO_DEFAULT_CONNECT_RE_TX_EFFORT                       (HCI_ESCO_POWER_SAVING_RETX)
#endif /* ESCO_DEFAULT_CONNECT_RE_TX_EFFORT */

/* 3M EDR eSCO packets cannot be used on BC4 with CVSD.  This value
 * can be changed to 0x003F if using BC5 or newer, or if another
 * aircoding is used. */
#ifndef ESCO_DEFAULT_CONNECT_AUDIO_QUALITY
#define ESCO_DEFAULT_CONNECT_AUDIO_QUALITY (0x02BF)
#endif /* ESCO_DEFAULT_CONNECT_AUDIO_QUALITY */

/* Default packet type for accept of incoming SCO and eSCO (S1, S2 ans S3) */
#ifndef COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY
#define COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY               (0x038F) /* HV1 + HV2 + HV3 + EV3 + not 3-EV3 + not 2-EV5 + not 3-EV5 */
#endif /* COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY */

/*----------------------------------------------------------------------------
 * Defines for JSR82
 *----------------------------------------------------------------------------*/

/* This is the max number of bytes the JSR82 layer will accept from
 * the SD in one SD_READ_TRUSTED_DEVICES_IND/CFM signal */
#ifndef JSR82_MAX_BYTES_IN_SD_CACHE_READ
#define JSR82_MAX_BYTES_IN_SD_CACHE_READ                        (2000)
#endif /* JSR82_MAX_BYTES_IN_SD_CACHE_READ */

/* This is the time in seconds that the JSR-82 layer uses for the
 * inquiry procedure */
#ifndef JSR82_INQ_TIME
#define JSR82_INQ_TIME                                          (11)
#endif /* JSR82_INQ_TIME */

/* This is the max number of results that should be returned during
 * an inquiry in JSR-82. 0 means no limit. */
#ifndef JSR82_INQ_MAX_RESULTS
#define JSR82_INQ_MAX_RESULTS                                   (0)
#endif /* JSR82_INQ_MAX_RESULTS */

 /* Size in bytes of the circular buffer for JSR-82 RFCOMM connections.
 * Should be greater than PROFILE_DEFAULT_MAX_FRAME_SIZE */
#ifndef JSR82_RFC_BUFFER_SIZE
#define JSR82_RFC_BUFFER_SIZE                                   ((uint32_t)(5*PROFILE_DEFAULT_MAX_FRAME_SIZE))
#endif /* JSR82_RFC_BUFFER_SIZE */

/* Max number of packets to store in the receive buffer for a JSR-82
 * L2CAP connection. If more than this is received, the oldest packets
 * will be dropped */
#ifndef JSR82_MAX_UNPROCESSED_DATA_PACKETS
#define JSR82_MAX_UNPROCESSED_DATA_PACKETS                      ((uint32_t)25)
#endif /* JSR82_MAX_UNPROCESSED_DATA_PACKETS */

/* The maximum number of dynamic range PSMs that may be allocated to the JSR82 task */
#ifndef JSR82_MAX_ALLOCATED_PSMS
#define JSR82_MAX_ALLOCATED_PSMS                                (16)
#endif /* JSR82_MAX_ALLOCATED_PSMS */


/*--------------------------------------------------------------------------
 * Defines for BCHS TCS profile manager
 *--------------------------------------------------------------------------*/
/* 0x12 - 0x1000, recommended value max 480 slots = 300 ms */
#ifndef TCS_PAGE_SCAN_INTERVAL
#define TCS_PAGE_SCAN_INTERVAL                                  (480)
#endif /* TCS_PAGE_SCAN_INTERVAL */

/* window   0x12 - 0x1000, default 0x12 */
#ifndef TCS_PAGE_SCAN_WINDOW
#define TCS_PAGE_SCAN_WINDOW                                    (0x12)
#endif /* TCS_PAGE_SCAN_WINDOW */

/* max number of calls to handle simultaneously */
#ifndef TCS_MAX_NUMBER_OF_CALLS
#define TCS_MAX_NUMBER_OF_CALLS                                 (0x03)
#endif /* TCS_MAX_NUMBER_OF_CALLS */

/* minimum value according to ctp profile */
#ifndef TCS_CTP_MTU_SIZE
#define TCS_CTP_MTU_SIZE                                        (171)
#endif /* TCS_CTP_MTU_SIZE */

/* minimum value is 3 */
#ifndef TCS_ICP_MTU_SIZE
#define TCS_ICP_MTU_SIZE                                        (127)
#endif /* TCS_ICP_MTU_SIZE */

/* Max time in betw. reconnect attempt for default GW */
#ifndef LINK_GW_RECONNECT_MAX
#define LINK_GW_RECONNECT_MAX                                   (60)
#endif /* LINK_GW_RECONNECT_MAX */

/* Inc timer for reconnect (attempt * value) */
#ifndef LINK_GW_RECONNECT_INC
#define LINK_GW_RECONNECT_INC                                   (2)
#endif /* LINK_GW_RECONNECT_INC */

/* Initiate link low power modes if no call setup received and link not parked for ctp within */
#ifndef TCS_TIMER_CTP_LINK_SUPERVISION
#define TCS_TIMER_CTP_LINK_SUPERVISION                          ((TcsTimer_t) (15))
#endif /* TCS_TIMER_CTP_LINK_SUPERVISION */

/* Release link if no call setup received link not parked for ctp within */
#ifndef TCS_TIMER_CTP_LINK_SUPERVISION_DISCONNECT
#define TCS_TIMER_CTP_LINK_SUPERVISION_DISCONNECT               ((TcsTimer_t) (75))
#endif /* TCS_TIMER_CTP_LINK_SUPERVISION_DISCONNECT */

/* Try to set the link in low power mode if no data received within this time for new connections (GW side) */

/* Time in seconds in between consecutive re-connect from the TL
 * towards the GW upon disconnect or failed connect attempts.  Value
 * of zero means no connect or reconnect. TCS will connect upon a
 * setup request */
#ifndef TCS_TIMER_CTP_LINK_SUPERVISION_GW
#define TCS_TIMER_CTP_LINK_SUPERVISION_GW                       ((TcsTimer_t) (5))
#endif /* TCS_TIMER_CTP_LINK_SUPERVISION_GW */


/*--------------------------------------------------------------------------
 * Defines for BCHS BNEP connection
 *--------------------------------------------------------------------------*/
/* Defines the maximum number of packets allowed on queue for each
 * bnep connection */
#ifndef MAX_BSL_QUEUE_LENGTH
#define MAX_BSL_QUEUE_LENGTH                                    (8)
#endif /* MAX_BSL_QUEUE_LENGTH */

#ifndef MAX_BSL_LOCAL_MULTICAST_QUEUE_LENGTH
#define MAX_BSL_LOCAL_MULTICAST_QUEUE_LENGTH                    (8)
#endif /* MAX_BSL_LOCAL_MULTICAST_QUEUE_LENGTH */

#ifdef RFC_BUILD
#ifndef MAX_NUM_OF_SIMULTANIOUS_BNEP_CONNECTIONS
#define MAX_NUM_OF_SIMULTANIOUS_BNEP_CONNECTIONS                (4) /* most be aleast 4 for RFCOMM build */
#endif /* MAX_NUM_OF_SIMULTANIOUS_BNEP_CONNECTIONS */
#else
#ifndef MAX_NUM_OF_SIMULTANIOUS_BNEP_CONNECTIONS
#define MAX_NUM_OF_SIMULTANIOUS_BNEP_CONNECTIONS                (8) /* most be aleast 8 for HCI build*/
#endif /* MAX_NUM_OF_SIMULTANIOUS_BNEP_CONNECTIONS */
#endif /* RFC_BUILD */

/*--------------------------------------------------------------------------
 * Defines for the Link Supervision timeout
 *--------------------------------------------------------------------------*/

/* Define the default link supervision timeout. Range is 0x0001 -
 * 0xFFFF and default is 0x7D00 (32000 ~ 20 seconds).  time_in_seconds
 * = DEFAULT_LINK_SUPERVISION_TIMEOUT * 0.000625 0x0000 means infinite
 * link supervision timeout. */
#ifndef DEFAULT_LINK_SUPERVISION_TIMEOUT
#define DEFAULT_LINK_SUPERVISION_TIMEOUT                        (32000)
#endif /* DEFAULT_LINK_SUPERVISION_TIMEOUT */

/*--------------------------------------------------------------------------
 * Defines for BCHS OBEX parameter
 *--------------------------------------------------------------------------*/

/* The obex max packet size received from the peer side will be this
 * value - 1024 dec.  Please notice that the largest acceptable packet
 * size value for IrOBEX is 0xFFFF, and the minimum value is
 * 0x00FF. */
#ifndef MAX_OBEX_SIGNAL_LENGTH
#define MAX_OBEX_SIGNAL_LENGTH                                  16384
#endif /* MAX_OBEX_SIGNAL_LENGTH */

#ifndef OBEX_CONNECT_TIMEOUT               
#define OBEX_CONNECT_TIMEOUT                                    0 /* Mircosec */
#endif /* OBEX_CONNECT_TIMEOUT (Recommended value if used is 5000000) */

#ifndef OBEX_CONNECT_WITH_AUTH_TIMEOUT               
#define OBEX_CONNECT_WITH_AUTH_TIMEOUT                         0 /* Mircosec */
#endif /* OBEX_CONNECT_WITH_AUTH_TIMEOUT (Recommended value if used is 20000000)*/

#ifndef OBEX_DISCONNECT_TIMEOUT               
#define OBEX_DISCONNECT_TIMEOUT                                 0 /* Mircosec */
#endif /* OBEX_DISCONNECT_TIMEOUT (Recommended value if used is 5000000)*/

/*--------------------------------------------------------------------------
 * Defines for FM Radio
 *--------------------------------------------------------------------------*/

/* FM_EU_US_BAND uses european / U.S. FM band (87.5 - 108 MHz)
 * FM_JAPAN_BAND uses japanese FM band (76 - 90 MHz) */
#ifndef FM_DEFAULT_BAND
#define FM_DEFAULT_BAND                                         FM_EU_US_BAND
#endif /* FM_DEFAULT_BAND */

/* RDS_SYSTEM uses the european standard
 * RBDS_SYSTEM uses the U.S. standard */
#ifndef FM_DEFAULT_RDS_SYSTEM
#define FM_DEFAULT_RDS_SYSTEM                                   FM_RDS_SYSTEM
#endif /* FM_DEFAULT_RDS_SYSTEM */

/* The default value of the inter frequency(IF)count is +-15 kHz.
 * An IF count outside this range will normally indicate that the receiver is
 * tuned to one side of the wanted channel
 * The valid range is [0:255] kHz, but given that the default station spacing
 * is 100 kHz a value above 50 kHz could lead to strange behaviour
 */
#ifndef FM_DEFAULT_IF_LIMIT
#define FM_DEFAULT_IF_LIMIT                                     ((uint8_t) 15)
#endif /* FM_DEFAULT_IF_LIMIT */

/* The nominal sensitivity of the FM receiver is -107 dB. A channel with a RSSI
 * below the receiver's nominal sensitivity will not be reliably received
 * The default value should be -107 dB. The range is [-127:127] dB
 */
#ifndef FM_DEFAULT_RSSI_LIMIT
#define FM_DEFAULT_RSSI_LIMIT                                   ((int8_t) -107)
#endif /* FM_DEFAULT_RSSI_LIMIT */

/* A search level of -80 dB has proved to be a good compromise choosing only
 * stations that will give good performance and rejecting good stations. Using
 * a lower (more negative) value will cause tuning to stop at more stations
 * but will increase the probability of stopping at a noisy station, using a
 * higher (less negative) value will increase the likelihood of missing good
 * stations.  The range is [-127:127] dB
 */
#ifndef FM_DEFAULT_SEARCH_LEVEL
#define FM_DEFAULT_SEARCH_LEVEL                                 ((int8_t) -80)
#endif /* FM_DEFAULT_SEARCH_LEVEL */

/* A default pause level of 640 kHz is recommended. A pause will be detected
 * if it below the level and of a duration longer than specified in
 * PAUSE_DURATION.
 * The range is from 0 kHz to 8160 kHz in 32 kHz intervals.
 */
#ifndef FM_DEFAULT_PAUSE_LEVEL
#define FM_DEFAULT_PAUSE_LEVEL                                 ((uint16_t) 640)
#endif /* FM_DEFAULT_PAUSE_LEVEL */

/* A default pause duration of 30 ms is recommended. A pause will be detected
 * if it below the pause level and of a duration longer
 * The range is from 0 ms to 189 ms in 3 ms intervals.
 */
#ifndef FM_DEFAULT_PAUSE_DURATION
#define FM_DEFAULT_PAUSE_DURATION                              ((uint8_t) 30)
#endif /* FM_DEFAULT_PAUSE_DURATION */


/* When rds is requested a time out has to be defined if no valid RDS
 * data has been received in a given time the autosearch function stop
 * waiting and continue the search */
#ifndef FULLAUTOSEARCHRDSTIMEOUT
#define FULLAUTOSEARCHRDSTIMEOUT                               (2000000)
#endif /* FULLAUTOSEARCHRDSTIMEOUT */

/* During the alternate frequency (AF) jumping algorithm two timers are
 * used to control the time between attempts of AF jumps.
 *
 *  FM_AF_JUMP_TIME_INTERVAL: When looping through the AF list this define
 *  controls the minimum time between AF jump attempts.
 *
 *  FM_AF_SCAN_TIME_INTERVAL: If the scan of the AF list did not
 *  produce a satisfactory station (i.e. RSSI level is lower than requested)
 *  the AF jump algorithm will attempt to go through the AF list again.
 *  This define controls the minimum time before the AF jump algorithm
 *  will attempt to go through the AF list again.
 *
 *  The times are in micro seconds.
 */

#ifndef FM_AF_JUMP_TIME_INTERVAL
#define FM_AF_JUMP_TIME_INTERVAL                                (5000000)
#endif /* FM_AF_JUMP_TIME_INTERVAL */

#ifndef FM_AF_SCAN_TIME_INTERVAL
#define FM_AF_SCAN_TIME_INTERVAL                                (5000000)
#endif /* FM_AF_SCAN_TIME_INTERVAL */

/* The accepted amount of errors in the RDS block
 * The recommended value is  RDS_DETECTED_UNCORRECTABLE_ERRORS which means
 * that the onchip RDS error correction algorithm believe that it has corrected
 * any errors there might have been.
 *
 * Alternative settings are:
 *  RDS_DETECTED_NO_ERRORS                
 *  RDS_DETECTED_UPTO_TWO_CORRECTED_ERRORS 
 *  RDS_DETECTED_UPTO_FIVE_CORRECTED_ERRORS
 *
 * The alternative settings can cause the RDS collection time to increase 
 * significantly, even to the point of preventing RDS collection all together
 */

#ifndef FM_RDS_DETECTED_MAX_ERRORS
#define FM_RDS_DETECTED_MAX_ERRORS           RDS_DETECTED_UNCORRECTABLE_ERRORS
#endif /* RDS_DETECTED_MAX_ERRORS */


/* The audio io setting for  
 *
 * Alternative settings are:
 *  1: Analog
 *  2: Digital
 *  3: Analog+Digital
 *
*/
#ifndef FM_DEFAULT_AUDIO_IO
#define FM_DEFAULT_AUDIO_IO         ((uint8_t)1) 
#endif /* FM_DEFAULT_AUDIO_IO */

/*--------------------------------------------------------------------------
 * Defines for FM TX Radio
 *--------------------------------------------------------------------------*/

/* The audio io setting for source selection.
 * The FM transmitter input may be routed from a digital or an analogue source.
 *
 * Alternative settings are:
 *  SRC_ANALOGUE
 *  SRC_DIGITAL
 *
*/
#ifndef FM_TX_DEFAULT_AUDIO_IO   
#define FM_TX_DEFAULT_AUDIO_IO      (SRC_ANALOGUE)
#endif /* FM_TX_DEFAULT_AUDIO_IO */

/* The audio range allows the setting of the analogue gains for the audio input
 * 
 * Values betweeen -40 dB and +21.5 dB with a resolution of 2.8 dB
 *
 * The recommended value is -6.4 dB aka step ....
 *  
 * The following points are given -6.4dB: 300 mV, -3.6dB: 200mv, 2dB: 100mv 
 *                      step:     12               13            15   
 *
 * The value set below must be converted to a uint16_t according to the 
 * following formula:
 *  
 * stepValue = (value_in_dB + 40 dB)/2.8 dB/step
*/

#ifndef FM_TX_DEFAULT_AUDIO_RANGE
#define FM_TX_DEFAULT_AUDIO_RANGE   ((uint16_t) 12) 
/* (-6.4 dB + 40 dB)/2.8 db/step = 12 */
#endif /* FM_TX_DEFAULT_AUDIO_RANGE */

/* The power level of the transmitters power amplifier stage with a 1dB resolution
 *  
 * The absolute transmitted power will depend on the antenna design
 *
*/
#ifndef FM_TX_DEFAULT_POWER_LEVEL
#define FM_TX_DEFAULT_POWER_LEVEL   ((int16_t)-60)
#endif /* FM_TX_DEFAULT_POWER_LEVEL */

/* The transmitter's audio deviation
 * The approximate range is 0-90 kHz with a resolution of 0.35 kHz
 * 
 * The recommended value is 68.1 kHz aka step 195
 *
 * The value set below must be converted to a uint16_t according to the 
 * following formula:
 *  
 * stepValue = value_in_kHz/0.35
*/
#ifndef FM_TX_DEFAULT_AUDIO_DEV
#define FM_TX_DEFAULT_AUDIO_DEV     ((uint16_t)195) 
/* 68.1kHz / 0.35 kHz/step = 195 */
#endif /* FM_TX_DEFAULT_AUDIO_DEV */

/* The transmitter's pilot deviation 
 * The approximate range is 0-10 kHz with a resolution of 41 Hz
 * 
 * The recommended value is 6.75 kHz aka step 165
 *
 * The value set below must be converted to a uint16_t according to the 
 * following formula:
 *  
 * stepValue = value_in_kHz *1000 / 41
 * 
 */
#ifndef FM_TX_DEFAULT_PILOT_DEV
#define FM_TX_DEFAULT_PILOT_DEV     ((uint16_t) 165) 
/* 6.75 kHz *1000/41 = 165*/
#endif /* FM_TX_DEFAULT_PILOT_DEV */

/* The dynamic range compresser (DRC) settings  
 *  
 *  EN: The dynamic range compressor is enabled
 *  SK: The soft-knee in the dynamic range compressor is enabled
 * 
 *   Note: the detailed behavour of DRC is controlled via PSKEYs
*/
#ifndef FM_TX_DEFAULT_DRC_MODE
#define FM_TX_DEFAULT_DRC_MODE      (EN + SK)
#endif /* FM_TX_DEFAULT_DRC_MODE */

/* Pre-emphasis filter
 * 
 * PREEN: Enable the pre-emphasis filter
 * PRETC: Set the re-emphasis filter's time constant to 75 us
 * Not PRETC: Set the re-emphasis filter's time constant to 50 us
*/
#ifndef FM_TX_DEFAULT_PREEMP
#define FM_TX_DEFAULT_PREEMP        (PREEN + PRETC)
#endif /* FM_TX_DEFAULT_PREEMP */

/* The transmitter's multiplex limiter. This allows the frequency deviation of the 
 * stereo multiplex to be clipped against an absolute maximum. 
 * 
 * 0 enable the transmitter's multiplex limiter
 * 1 disable the transmitter's multiplex limiter
 */
#ifndef FM_TX_DEFAULT_MXP_LIMIT
#define FM_TX_DEFAULT_MXP_LIMIT     (1)
#endif /* FM_TX_DEFAULT_MXP_LIMIT */


/* The transmitter operates in mono or stereo mode
 *  settings: MO_STEREO, MO_MONO
 */
#ifndef FM_TX_DEFAULT_MONO_STEREO_MODE
#define FM_TX_DEFAULT_MONO_STEREO_MODE  (MO_STEREO)
#endif /* FM_TX_DEFAULT_MONO_STEREO_MODE */

/*  RDS Programs Service name (PS) display mode 
 *
 * 0  PS is sent as a static string  
 * 1  PS is sent as a scrolling string, advancing by one character per repetition
 * 2  PS is sent as a paged string, updating in 8 character blocks
 * 
 * Note, setting 1 and 2 may contravene the RDS specification, as they may cause
 * user distraction
 */
#ifndef FM_TX_DEFAULT_DISP_MODE 
#define FM_TX_DEFAULT_DISP_MODE     (0)
#endif /* FM_TX_DEFAULT_DISP_MODE */

/*--------------------------------------------------------------------------
 * Defines for BCSP
 *--------------------------------------------------------------------------*/
#ifndef BCSP_RETRANSMISSION_TIMER
#define BCSP_RETRANSMISSION_TIMER                               (250000)
#endif /* BCSP_RETRANSMISSION_TIMER */


/* The size of the BCSP transmit window.  This must be between 1 and 7.  This
is normally set to 4.  This is called "winsiz" in the BCSP protocol
specification.

This determines the number of BCSP messages that can be handled by the abcsp
library's transmit path at a time, so it affects the storage requirements for
ABCSP_TXMSG messages. */

#ifndef ABCSP_TXWINSIZE
#define ABCSP_TXWINSIZE                                         (4)
#endif /* ABCSP_TXWINSIZE */

/* Number of USB devices to support in pure user mode */
#ifndef BT_USB_COUNT
#define BT_USB_COUNT                                        (31)
#endif /* BT_USB_COUNT */
/*--------------------------------------------------------------------------
 * Defines for the Linux port
 *--------------------------------------------------------------------------*/

/* Setup which PCM SLOT the AV2ALSA device is allocated to */
#ifndef AV2ALSA_PCM_SLOT
#define AV2ALSA_PCM_SLOT                                        (0)
#endif /* AV2ALSA_PCM_SLOT */

/*--------------------------------------------------------------------------
 * Defines for AVRCP
 *--------------------------------------------------------------------------*/

/* Enable automatic use of link-mode for non-streaming connections */
#ifndef AVRCP_ENABLE_SNIFF
#define AVRCP_ENABLE_SNIFF
#endif /* AVRCP_ENABLE_SNIFF */

/* The following define is reserved for future use and must be set to 1*/
#ifndef AVRCP_MAX_NUM_OF_CONNECTIONS
#define AVRCP_MAX_NUM_OF_CONNECTIONS                            (1)
#endif /* AVRCP_MAX_NUM_OF_CONNECTIONS */

/*--------------------------------------------------------------------------
 * Defines for AV
 *--------------------------------------------------------------------------*/

/* Supported features to be advertised in A2DP service record. Sink
 * feature values */
#ifndef AV_FEATURES_HEADPHONE
#define AV_FEATURES_HEADPHONE                                0x0001
#endif /* AV_FEATURES_HEADPHONE */

#ifndef AV_FEATURES_SPEAKER
#define AV_FEATURES_SPEAKER                                  0x0002
#endif /* AV_FEATURES_SPEAKER */

#ifndef AV_FEATURES_RECORDER
#define AV_FEATURES_RECORDER                                 0x0004
#endif /* AV_FEATURES_RECORDER */

#ifndef AV_FEATURES_AMPLIFIER
#define AV_FEATURES_AMPLIFIER                                0x0008
#endif /* AV_FEATURES_AMPLIFIER */

/* Source feature values */
#ifndef AV_FEATURES_PLAYER
#define AV_FEATURES_PLAYER                                   0x0001
#endif /* AV_FEATURES_PLAYER */

#ifndef AV_FEATURES_MICROPHONE
#define AV_FEATURES_MICROPHONE                               0x0002
#endif /* AV_FEATURES_MICROPHONE */

#ifndef AV_FEATURES_TUNER
#define AV_FEATURES_TUNER                                    0x0004
#endif /* AV_FEATURES_TUNER */

#ifndef AV_FEATURES_MIXER
#define AV_FEATURES_MIXER                                    0x0008
#endif /* AV_FEATURES_MIXER */

/* Specify supported features by combining above values */
#ifndef AV_SUPPORTED_FEATURES
#define AV_SUPPORTED_FEATURES                                0x0000
#endif /* AV_SUPPORTED_FEATURES */

/* Product/provider specific names */
#ifndef AV_PROVIDER_NAME
#define AV_PROVIDER_NAME                                     "CSR"
#endif /* AV_PROVIDER_NAME */

#ifndef AV_SERVICE_NAME
#define AV_SERVICE_NAME                                      "Audio/Video Service"
#endif /* AV_SERVICE_NAME */

/* the maximum number of concurrent AV connections */
#ifndef AV_MAX_NUM_CONNECTIONS
#define AV_MAX_NUM_CONNECTIONS      3
#endif /* AV_MAX_NUM_CONNECTIONS */

/* the maximum number of concurrent AV streams, should at
   least be same value as AV_MAX_NUM_CONNECTIONS */
#ifndef AV_MAX_NUM_STREAMS
#define AV_MAX_NUM_STREAMS          4
#endif /* AV_MAX_NUM_STREAMS */

/* size of the media stream buffer, maximum number of media
   packets that can be buffered before flushing oldest data.
   Should be able to hold at least 50 ms data, must be >2 */
#ifndef AV_MEDIA_BUFFER_SIZE
#define AV_MEDIA_BUFFER_SIZE        100
#endif /* AV_MEDIA_BUFFER_SIZE */

/* how often should AV report the buffer status (AV_QOS_IND) in
   counts of AV_STREAM_DATA_REQ primitives sent. If set to 0 (zero)
   an AV_QOS_IND will only be sent in case the buffer is full
   and subsequently when the buffer again is emptied */
#ifndef AV_QOS_IND_INTERVAL
#define AV_QOS_IND_INTERVAL         50
#endif /* AV_QOS_IND_INTERVAL */

/* Timer T-gavdp100 duration in milliseconds (500 - 3000 ms recommended) */
#ifndef AV_CMD_SIGNAL_TIMEOUT
#define AV_CMD_SIGNAL_TIMEOUT       3000
#endif /* AV_CMD_SIGNAL_TIMEOUT */

/* Timer supervising signalling activity - if no signalling traffic has happened within the
    specified interval, the profile will request sniff mode on the signalling channel.
    Default 5000 msec. */
#ifndef AV_SIGNAL_LP_TIMEOUT
#define AV_SIGNAL_LP_TIMEOUT            5000
#endif /* AV_SIGNAL_LP_TIMEOUT */

/* Timer supervising stream activity - trigger entering of sniff mode when
   stream is inactive for the time specified here. Default 3000 msec.
   This timer value is only applicable when AV_STREAM_INACTIVITY_LP_ENABLE
   is defined */
#ifdef AV_STREAM_INACTIVITY_LP_ENABLE
#ifndef AV_LP_TIMEOUT
#define AV_LP_TIMEOUT            3000
#endif /* AV_LP_TIMEOUT */
#endif

/*--------------------------------------------------------------------------
 * Defines for BCHS security
 *--------------------------------------------------------------------------*/

/* Set the default sec level to mode 4 (Only applicable for BLUETOOTH_VERSION_2P1) */
#if (BCHS_BT_VERSION >= BLUETOOTH_VERSION_2P1)
#ifndef DEFAULT_SEC_MODE
#define DEFAULT_SEC_MODE                                        SEC_MODE4_SSP
#endif /* DEFAULT_SEC_MODE */
#else
#ifndef DEFAULT_SEC_MODE
#define DEFAULT_SEC_MODE                                        SEC_MODE2_SERVICE
#endif /* DEFAULT_SEC_MODE */
#endif

/* Set the default encryption mode for security mode 3. Please note
 * that this value is only used for security mode 3.  It can any one
 * of:
 * - HCI_ENC_MODE_OFF
 * - HCI_ENC_MODE_PT_TO_PT
 * - HCI_ENC_MODE_PT_TO_PT_AND_BCAST */
#ifndef DEFAULT_ENC_MODE3
#define DEFAULT_ENC_MODE3                                       (HCI_ENC_MODE_PT_TO_PT)
#endif /* DEFAULT_ENC_MODE3 */

/* The default application layer above the SC profile manager. Used if
 * no other handle is given when the function is activated */
#ifndef APPLICATION_SECURITY_HANDLER
#define APPLICATION_SECURITY_HANDLER                            TESTQUEUE
#endif /* APPLICATION_SECURITY_HANDLER */

/* Set the time window in which the user has to respond to a
 * SC_PASSKEY_IND, if the user fails to respond within this time
 * the SC will automatically respond the passkey indication with
 * error. The time is given in mSec and must not be higher than
 * 29000 mSec. Default 28000 mSec. */
#ifndef SC_PASSKEY_RESPONSE_TIMEOUT
#define SC_PASSKEY_RESPONSE_TIMEOUT                             28000
#endif /* SC_PASSKEY_RESPONSE_TIMEOUT */

/* Set default event mask */
#ifndef SC_DEFAULT_EVENT_MASK
#define SC_DEFAULT_EVENT_MASK                                   (SC_EVENT_MASK_NONE)
#endif /* SC_DEFAULT_EVENT_MASK */

/* Set default IO capability to a fully functional device */
#ifndef DEFAULT_IO_CAPABILITY
#define DEFAULT_IO_CAPABILITY                                   (HCI_IO_CAP_DISPLAY_YES_NO)
#endif /* DEFAULT_IO_CAPABILITY */

/* Set default authentication requirements */
#ifndef DEFAULT_AUTH_REQUIREMENTS
#define DEFAULT_AUTH_REQUIREMENTS                               (HCI_MITM_NOT_REQUIRED_GENERAL_BONDING)
#endif /* DEFAULT_AUTH_REQUIREMENTS */

/* Set default SC mode */
#ifndef DEFAULT_SC_MODE
#define DEFAULT_SC_MODE                                         (SEC_MODE_PAIRABLE)
#endif /* DEFAULT_SC_MODE */

#ifndef _DEFAULT_INCOMING_SECURITY_
#define _DEFAULT_INCOMING_SECURITY_                             (SEC_SPECIFY | \
                                                                 SEC_AUTHENTICATION |\
                                                                 SEC_ENCRYPTION     |\
                                                                 SEC_AUTHORISATION)
#endif /* _DEFAULT_INCOMING_SECURITY_ */

#ifndef _DEFAULT_OUTGOING_SECURITY_
#define _DEFAULT_OUTGOING_SECURITY_                             (SEC_SPECIFY | \
                                                                 SEC_AUTHENTICATION |\
                                                                 SEC_ENCRYPTION)
#endif /* _DEFAULT_OUTGOING_SECURITY_ */

/* Security levels for each profile, incoming connections */
#ifndef DUN_GW_DEFAULT_SECURITY_INCOMING
#define DUN_GW_DEFAULT_SECURITY_INCOMING                        (_DEFAULT_INCOMING_SECURITY_)
#endif /* DUN_GW_DEFAULT_SECURITY_INCOMING */

#ifndef FAX_GW_DEFAULT_SECURITY_INCOMING
#define FAX_GW_DEFAULT_SECURITY_INCOMING                        (_DEFAULT_INCOMING_SECURITY_)
#endif /* FAX_GW_DEFAULT_SECURITY_INCOMING */

#ifndef HANDSFREE_DEFAULT_SECURITY_INCOMING
#define HANDSFREE_DEFAULT_SECURITY_INCOMING                     (_DEFAULT_INCOMING_SECURITY_)
#endif /* HANDSFREE_DEFAULT_SECURITY_INCOMING */

#ifndef HANDSFREE_GW_DEFAULT_SECURITY_INCOMING
#define HANDSFREE_GW_DEFAULT_SECURITY_INCOMING                  (_DEFAULT_INCOMING_SECURITY_)
#endif /* HANDSFREE_GW_DEFAULT_SECURITY_INCOMING */

#ifndef OBEX_IMAGING_RESPONDER_DEFAULT_SECURITY_INCOMING
#define OBEX_IMAGING_RESPONDER_DEFAULT_SECURITY_INCOMING        (_DEFAULT_INCOMING_SECURITY_)
#endif /* OBEX_IMAGING_RESPONDER_DEFAULT_SECURITY_INCOMING */

#ifndef OBEX_OBJECT_PUSH_DEFAULT_SECURITY_INCOMING
#define OBEX_OBJECT_PUSH_DEFAULT_SECURITY_INCOMING              (_DEFAULT_INCOMING_SECURITY_)
#endif /* OBEX_OBJECT_PUSH_DEFAULT_SECURITY_INCOMING */

#ifndef OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING
#define OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING               (_DEFAULT_INCOMING_SECURITY_)
#endif /* OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING */

#ifndef OBEX_FILE_TRANSFER_DEFAULT_SECURITY_INCOMING
#define OBEX_FILE_TRANSFER_DEFAULT_SECURITY_INCOMING            (_DEFAULT_INCOMING_SECURITY_)
#endif /* OBEX_FILE_TRANSFER_DEFAULT_SECURITY_INCOMING */

#ifndef SIM_ACCESS_DEFAULT_SECURITY_INCOMING
#define SIM_ACCESS_DEFAULT_SECURITY_INCOMING                    (_DEFAULT_INCOMING_SECURITY_)
#endif /* SIM_ACCESS_DEFAULT_SECURITY_INCOMING */

#ifndef SERIAL_PORT_DEFAULT_SECURITY_INCOMING
#define SERIAL_PORT_DEFAULT_SECURITY_INCOMING                   (_DEFAULT_INCOMING_SECURITY_)
#endif /* SERIAL_PORT_DEFAULT_SECURITY_INCOMING */

#ifndef PBAP_DEFAULT_SECURITY_INCOMING
#define PBAP_DEFAULT_SECURITY_INCOMING                          (_DEFAULT_INCOMING_SECURITY_)
#endif /* PBAP_DEFAULT_SECURITY_INCOMING */

#ifndef BPPS_DEFAULT_SECURITY_INCOMING
#define BPPS_DEFAULT_SECURITY_INCOMING                          (_DEFAULT_INCOMING_SECURITY_)
#endif /* BPPS_DEFAULT_SECURITY_INCOMING */

#ifndef AV_DEFAULT_SECURITY_INCOMING
#define AV_DEFAULT_SECURITY_INCOMING                            (_DEFAULT_INCOMING_SECURITY_)
#endif /* AV_DEFAULT_SECURITY_INCOMING */

#ifndef AV_RCP_DEFAULT_SECURITY_INCOMING
#define AV_RCP_DEFAULT_SECURITY_INCOMING                        (_DEFAULT_INCOMING_SECURITY_)
#endif /* AV_RCP_DEFAULT_SECURITY_INCOMING */

#ifndef HIDH_DEFAULT_SECURITY_INCOMING
#define HIDH_DEFAULT_SECURITY_INCOMING                          (_DEFAULT_INCOMING_SECURITY_)
#endif /* HIDH_DEFAULT_SECURITY_INCOMING */

#ifndef HIDD_DEFAULT_SECURITY_INCOMING
#define HIDD_DEFAULT_SECURITY_INCOMING                          (_DEFAULT_INCOMING_SECURITY_)
#endif /* HIDD_DEFAULT_SECURITY_INCOMING */

#ifndef CTP_DEFAULT_SECURITY_INCOMING
#define CTP_DEFAULT_SECURITY_INCOMING                           (_DEFAULT_INCOMING_SECURITY_)
#endif /* CTP_DEFAULT_SECURITY_INCOMING */

#ifndef ICP_DEFAULT_SECURITY_INCOMING
#define ICP_DEFAULT_SECURITY_INCOMING                           (_DEFAULT_INCOMING_SECURITY_)
#endif /* ICP_DEFAULT_SECURITY_INCOMING */

#ifndef PAN_DEFAULT_SECURITY_INCOMING
#define PAN_DEFAULT_SECURITY_INCOMING                           (_DEFAULT_INCOMING_SECURITY_)
#endif /* PAN_DEFAULT_SECURITY_INCOMING */

#ifndef HCRP_DEFAULT_SECURITY_INCOMING
#define HCRP_DEFAULT_SECURITY_INCOMING                          (_DEFAULT_INCOMING_SECURITY_)
#endif /* HCRP_DEFAULT_SECURITY_INCOMING */

#ifndef OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_INCOMING
#define OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_INCOMING          (_DEFAULT_INCOMING_SECURITY_)
#endif /* OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_INCOMING */

/* Security levels for each profile, outgoing connections */
#ifndef HANDSFREE_DEFAULT_SECURITY_OUTGOING
#define HANDSFREE_DEFAULT_SECURITY_OUTGOING                     (_DEFAULT_OUTGOING_SECURITY_)
#endif /* HANDSFREE_DEFAULT_SECURITY_OUTGOING */

#ifndef HANDSFREE_GW_DEFAULT_SECURITY_OUTGOING
#define HANDSFREE_GW_DEFAULT_SECURITY_OUTGOING                  (_DEFAULT_OUTGOING_SECURITY_)
#endif /* HANDSFREE_GW_DEFAULT_SECURITY_OUTGOING */

#ifndef OBEX_FILE_TRANSFER_DEFAULT_SECURITY_OUTGOING
#define OBEX_FILE_TRANSFER_DEFAULT_SECURITY_OUTGOING            (_DEFAULT_OUTGOING_SECURITY_)
#endif /* OBEX_FILE_TRANSFER_DEFAULT_SECURITY_OUTGOING */

#ifndef OBEX_OBJECT_PUSH_DEFAULT_SECURITY_OUTGOING
#define OBEX_OBJECT_PUSH_DEFAULT_SECURITY_OUTGOING              (_DEFAULT_OUTGOING_SECURITY_)
#endif /* OBEX_OBJECT_PUSH_DEFAULT_SECURITY_OUTGOING */

#ifndef OBEX_IMAGING_INITIATOR_DEFAULT_SECURITY_OUTGOING
#define OBEX_IMAGING_INITIATOR_DEFAULT_SECURITY_OUTGOING        (_DEFAULT_OUTGOING_SECURITY_)
#endif /* OBEX_IMAGING_INITIATOR_DEFAULT_SECURITY_OUTGOING */

#ifndef OBEX_PRINTING_INITIATOR_DEFAULT_SECURITY_OUTGOING
#define OBEX_PRINTING_INITIATOR_DEFAULT_SECURITY_OUTGOING       (_DEFAULT_OUTGOING_SECURITY_)
#endif /* OBEX_PRINTING_INITIATOR_DEFAULT_SECURITY_OUTGOING */

#ifndef SIM_ACCESS_DEFAULT_SECURITY_OUTGOING
#define SIM_ACCESS_DEFAULT_SECURITY_OUTGOING                    (_DEFAULT_OUTGOING_SECURITY_)
#endif /* SIM_ACCESS_DEFAULT_SECURITY_OUTGOING */

#ifndef SERIAL_PORT_DEFAULT_SECURITY_OUTGOING
#define SERIAL_PORT_DEFAULT_SECURITY_OUTGOING                   (_DEFAULT_OUTGOING_SECURITY_)
#endif /* SERIAL_PORT_DEFAULT_SECURITY_OUTGOING */

#ifndef AV_DEFAULT_SECURITY_OUTGOING
#define AV_DEFAULT_SECURITY_OUTGOING                            (_DEFAULT_OUTGOING_SECURITY_)
#endif /* AV_DEFAULT_SECURITY_OUTGOING */

#ifndef AV_RCP_DEFAULT_SECURITY_OUTGOING
#define AV_RCP_DEFAULT_SECURITY_OUTGOING                        (_DEFAULT_OUTGOING_SECURITY_)
#endif /* AV_RCP_DEFAULT_SECURITY_OUTGOING */

#ifndef HIDH_DEFAULT_SECURITY_OUTGOING
#define HIDH_DEFAULT_SECURITY_OUTGOING                          (_DEFAULT_OUTGOING_SECURITY_)
#endif /* HIDH_DEFAULT_SECURITY_OUTGOING */

#ifndef HIDD_DEFAULT_SECURITY_OUTGOING
#define HIDD_DEFAULT_SECURITY_OUTGOING                          (_DEFAULT_OUTGOING_SECURITY_)
#endif /* HIDD_DEFAULT_SECURITY_OUTGOING */

#ifndef CTP_DEFAULT_SECURITY_OUTGOING
#define CTP_DEFAULT_SECURITY_OUTGOING                           (_DEFAULT_OUTGOING_SECURITY_)
#endif /* CTP_DEFAULT_SECURITY_OUTGOING */

#ifndef ICP_DEFAULT_SECURITY_OUTGOING
#define ICP_DEFAULT_SECURITY_OUTGOING                           (_DEFAULT_OUTGOING_SECURITY_)
#endif /* ICP_DEFAULT_SECURITY_OUTGOING */

#ifndef PAN_DEFAULT_SECURITY_OUTGOING
#define PAN_DEFAULT_SECURITY_OUTGOING                           (_DEFAULT_OUTGOING_SECURITY_)
#endif /* PAN_DEFAULT_SECURITY_OUTGOING */

#ifndef PBAP_DEFAULT_SECURITY_OUTGOING
#define PBAP_DEFAULT_SECURITY_OUTGOING                          (_DEFAULT_OUTGOING_SECURITY_)
#endif /* PBAP_DEFAULT_SECURITY_OUTGOING */

#ifndef DUN_CLIENT_DEFAULT_SECURITY_OUTGOING
#define DUN_CLIENT_DEFAULT_SECURITY_OUTGOING                    (_DEFAULT_OUTGOING_SECURITY_)
#endif /* DUN_CLIENT_DEFAULT_SECURITY_OUTGOING */

#ifndef OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_OUTGOING
#define OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_OUTGOING          (_DEFAULT_OUTGOING_SECURITY_)
#endif /* OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_OUTGOING */

/*--------------------------------------------------------------------------
 * Device Manager configuration
 *--------------------------------------------------------------------------*/

/* Set powerup timeout, which comes into play when we haven't received a
 * HCI_NOP from the controller. After the timeout has occurred BCHS will
 * start communicating with the controller even though a HCI_NOP has
 * not been received. */
#ifndef POWERUP_TIMEOUT
#define POWERUP_TIMEOUT                 ((TIME)(30 * COAL_SECOND))
#endif /* POWERUP_TIMEOUT */

/* Security detach timer. Allows enough time for encryption event to
 arrive before detaching the ACL. In 1/10s of a second. */
#ifndef DM_CONFIG_SM_DETACH_TIMEOUT
#define DM_CONFIG_SM_DETACH_TIMEOUT 20
#endif

/* Authenitcation repeat timer. Allows enough time before retrying
 authentication to avoid HCI_ERROR_AUTH_REPEATED failures.
 In 1/10s of a second. */
#ifndef DM_CONFIG_SM_REAUTHENTICATION_TIMEOUT
#define DM_CONFIG_SM_REAUTHENTICATION_TIMEOUT 10
#endif

/*--------------------------------------------------------------------------
 * Host to Host Controller flow control configuration
 *--------------------------------------------------------------------------*/

/* Defines for the size of the buffers in the host. These values should
 * be configured for the resources available on the host */
#ifndef RFC_BUILD
#ifdef EDR_DISABLE
#ifndef HOST_ACL_DATA_PACKET_LENGTH
#define HOST_ACL_DATA_PACKET_LENGTH                         ((uint16_t) 339)
#endif /* HOST_ACL_DATA_PACKET_LENGTH */
#else
#ifndef HOST_ACL_DATA_PACKET_LENGTH
#define HOST_ACL_DATA_PACKET_LENGTH                         ((uint16_t) 1021)
#endif /* HOST_ACL_DATA_PACKET_LENGTH */
#endif

#ifndef HOST_SCO_DATA_PACKET_LENGTH
#define HOST_SCO_DATA_PACKET_LENGTH                         ((uint8_t)  128)
#endif /* HOST_SCO_DATA_PACKET_LENGTH */

#ifndef HOST_TOTAL_NUM_ACL_DATA_PACKETS
#define HOST_TOTAL_NUM_ACL_DATA_PACKETS                     ((uint16_t) 100)
#endif /* HOST_TOTAL_NUM_ACL_DATA_PACKETS */

#ifndef HOST_TOTAL_NUM_SCO_DATA_PACKETS
#define HOST_TOTAL_NUM_SCO_DATA_PACKETS                     ((uint16_t) 16)
#endif /* HOST_TOTAL_NUM_SCO_DATA_PACKETS */

/* Defines for the thresholds that when hit a message is sent about processed data */
/* Note that these should be approx 50% of HOST_TOTAL_NUM_XXX_DATA_PACKETS */
#ifdef EDR_DISABLE
#ifndef HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION
#define HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION ((uint16_t) 8)
#endif /* HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION */
#else
#ifndef HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION
#define HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION ((uint16_t) 25)
#endif /* HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION */
#endif

#ifndef HOST_TOTAL_NUM_SCO_DATA_PACKETS_BEFORE_NOTIFICATION
#define HOST_TOTAL_NUM_SCO_DATA_PACKETS_BEFORE_NOTIFICATION ((uint16_t) 8)
#endif /* HOST_TOTAL_NUM_SCO_DATA_PACKETS_BEFORE_NOTIFICATION */

#else

#ifndef HOST_ACL_DATA_PACKET_LENGTH
#define HOST_ACL_DATA_PACKET_LENGTH                         ((uint16_t) 128)
#endif /* HOST_ACL_DATA_PACKET_LENGTH */

#ifndef HOST_SCO_DATA_PACKET_LENGTH
#define HOST_SCO_DATA_PACKET_LENGTH                         ((uint8_t) 128)
#endif /* HOST_SCO_DATA_PACKET_LENGTH */

#ifndef HOST_TOTAL_NUM_ACL_DATA_PACKETS
#define HOST_TOTAL_NUM_ACL_DATA_PACKETS                     ((uint16_t) 16)
#endif /* HOST_TOTAL_NUM_ACL_DATA_PACKETS */

#ifndef HOST_TOTAL_NUM_SCO_DATA_PACKETS
#define HOST_TOTAL_NUM_SCO_DATA_PACKETS                     ((uint16_t) 16)
#endif /* HOST_TOTAL_NUM_SCO_DATA_PACKETS */

#ifndef HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION
#define HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION ((uint16_t) 8)
#endif /* HOST_TOTAL_NUM_ACL_DATA_PACKETS_BEFORE_NOTIFICATION */

#ifndef HOST_TOTAL_NUM_SCO_DATA_PACKETS_BEFORE_NOTIFICATION
#define HOST_TOTAL_NUM_SCO_DATA_PACKETS_BEFORE_NOTIFICATION ((uint16_t )8)
#endif /* HOST_TOTAL_NUM_SCO_DATA_PACKETS_BEFORE_NOTIFICATION */

#endif

/*--------------------------------------------------------------------------
 * Host controller to Host flow control configuration
 *--------------------------------------------------------------------------*/
/* Defines for the size of the buffers in the controller. These values should
 * be configured for the resources available on the controller */
#ifndef HC_ACL_DATA_PACKET_LENGTH
#define HC_ACL_DATA_PACKET_LENGTH                           ((uint16_t) 342)
#endif /* HC_ACL_DATA_PACKET_LENGTH */

#ifndef HC_TOTAL_NUM_ACL_DATA_PACKETS
#define HC_TOTAL_NUM_ACL_DATA_PACKETS                       ((uint16_t) 9)
#endif /* HC_TOTAL_NUM_ACL_DATA_PACKETS */

/* Encryption retry timer. Allows the LM enough time to recover
 between encryption requests. */
#ifndef DM_CONFIG_SM_REENCRYPTION_TIMEOUT
#define DM_CONFIG_SM_REENCRYPTION_TIMEOUT                   (2)
#endif

/* Time in quarters (1/4) of a second between bonding completing successfully
 and the ACL being closed. If bonding fails or is cancelled then the ACL
 always closes immediately. This will not work in conjunction with
 powerstate tables and so should only be used by BCHS.

 The example here is for a 2 second timeout after bonding finishes.

#define DM_CONFIG_ACL_IDLE_TIMEOUT_BONDING                  ((uint32_t) ((2*COAL_SECOND)/(250*COAL_MILLISECOND)))
*/

/* Mode change holdoff times after a failed locally-initiated mode
 change request and after a remotely-initiated mode change. */
#ifndef DM_CONFIG_MODE_CHANGE_FAILED_HOLDOFF_TIME
#define DM_CONFIG_MODE_CHANGE_FAILED_HOLDOFF_TIME           (1 * COAL_SECOND)
#endif
#ifndef DM_CONFIG_MODE_CHANGE_REMOTE_HOLDOFF_TIME
#define DM_CONFIG_MODE_CHANGE_REMOTE_HOLDOFF_TIME           (5 * COAL_SECOND)
#endif

/*--------------------------------------------------------------------------
 * Defines for Audio Codec support (HFG/HF)
 *--------------------------------------------------------------------------*/
/* Supported codec mask
 * CVSD               : 0x01
 * ADPCM 2bit/sample  : 0x02
 * ADPCM 4bit/sample  : 0x04
*/
#ifndef AUDIO_CODEC_SUPPORT_MASK
#define AUDIO_CODEC_SUPPORT_MASK                            (0x01)
#endif /* AUDIO_CODEC_SUPPORT_MASK */

/* custom AT mode, default settings */
#define HFG_AT_CUSTOM_BYTE_0                                 (0x1D)
#define HFG_AT_CUSTOM_BYTE_1                                 (0xFC)
#define HFG_AT_CUSTOM_BYTE_2                                 (0x01)
#define HFG_AT_CUSTOM_BYTE_3                                 (0xFF)
#define HFG_AT_CUSTOM_BYTE_4                                 (0xFD)
#define HFG_AT_CUSTOM_UNUSED                                 (0xFF)

/*--------------------------------------------------------------------------
 * Defines for FTP (FTS/FTC)
 *--------------------------------------------------------------------------*/

#ifndef FTC_LP_SUPERVISION_TIMEOUT
#define FTC_LP_SUPERVISION_TIMEOUT      (4000000) /* Number of microseconds without data before entering sniff (if possible) */
#endif /* FTC_LP_SUPERVISION_TIMEOUT */

/*--------------------------------------------------------------------------
 * Defines for L2CAP
 *--------------------------------------------------------------------------*/
/*  L2CAP keeps an ACL open for a short period after the last L2CAP connection was closed.  This allows L2CAP
    connections opened soon after to use the same ACL without having to wait for it to re-open.
*/
#define L2CAP_ACL_IDLE_TIMEOUT                              (TIME)(250*COAL_MILLISECOND)
#define L2CAP_ACL_IDLE_TIMEOUT_RESPONDER                    (TIME)(30*COAL_SECOND)

/*  The L2CAP Response Timeout eXpired (RTX) timer is used to terminate the channel
    when the remote endpoint is unresponsive to signaling requests. 
    The value of this timer is implementation-dependent but the minimum initial value 
    is 1 second and the maximum initial value is 60 seconds */
#define L2CAP_RTX_TIMEOUT                                   (TIME)(10*COAL_SECOND)

/*  The L2CAP Extended Response Timeout eXpired (ERTX) timer is used in place of the
    RTX timer when it is suspected the remote endpoint is performing additional
    processing of a request signal. This timer is started when the remote endpoint
    responds that a request is pending.
    The value of this timer is implementation-dependent but the minimum initial
    value is 60 seconds and the maximum initial value is 300 seconds */
#define L2CAP_ERTX_TIMEOUT                                  (TIME)(60*COAL_SECOND)

/* Number of RTX and ERTX retries */
#define L2CAP_RTX_RETRIES                                   (1)
#define L2CAP_ERTX_RETRIES                                  (1)

#undef CONFIG_USB_SUSPEND
#ifdef __cplusplus
}
#endif

#endif

