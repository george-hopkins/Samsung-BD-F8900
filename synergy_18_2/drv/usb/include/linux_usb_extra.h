#ifndef LINUX_USB_EXTRA_H__
#define LINUX_USB_EXTRA_H__
/****************************************************************************

               (c) CSR plc 2008

               All rights reserved

FILE:          linux_usb_extra.h

DESCRIPTION:   This file specifies the interface that must be implemented if
               a special interface is used in addition to the defaults.

REVISION:      $Revision: #14 $

****************************************************************************/

#include "sched/csr_types.h"
#include "linux_usb_com.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************
 * NAME:
 *      probeExtraInterface
 *
 * DESCRIPTION:
 *      Is called by the usb probe function when having handled the
 *      default interfaces.
 *
 * RETURNS:
 *      TRUE   when interface has been probed by this function
 *      FALSE  if not probed.
 *
 *************************************************************/
typedef bool_t (*probeExtraInterface_t)(csr_dev_t *csr_dev,
                                        uint8_t intfNumber,
                                        struct usb_interface *intf,
                                        int altsetting);

/*************************************************************
 * NAME:
 *      setupExtraInterface
 *
 * DESCRIPTION:
 *      Is used to setup information about what bg_int number to
 *      use and which bg_int function to call, when data is received
 *      on the extra interface
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
typedef void (*setupExtraInterface_t)(uint8_t intrNr,
                                      void (*intrFunc)(void));

/*************************************************************
 * NAME:
 *      disconnectExtraInterface
 *
 * DESCRIPTION:
 *      Is called by the general usb_driver whne it disconnects
 *      and therefore some cleanup of the extra interface must
 *      be done.
 *
 * RETURNS:
 *      0 on success, otherwise non-zero for error
 *
 *************************************************************/
typedef int16_t (*disconnectExtraInterface_t)(void);

/*************************************************************
 * NAME:
 *      startDrvExtraInterface
 *
 * DESCRIPTION:
 *      Is called by the general usb_driver when the layers
 *      above calls UsbDrv_Start
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
typedef void (*startDrvExtraInterface_t)(void);

/*************************************************************
 * NAME:
 *      initExtraInterface
 *
 * DESCRIPTION:
 *      Must called from CsrUsbInit in order to init the extra
 *      usb driver
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
typedef void (*initExtraInterface_t)(void);

/*************************************************************
 * NAME:
 *      exitExtraInterface
 *
 * DESCRIPTION:
 *      Must called from CsrUsbExit in order to init the extra
 *      usb driver
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
typedef void (*exitExtraInterface_t)(void);

/*************************************************************
 * NAME:
 *      setupUsb
 *
 * DESCRIPTION:
 *      Must be called if an extra USB interface is to be used.
 *      It setups the function necessary for the extra interface to
 *      work properly.
 *
 * PARAMETERS:
 *      intrNr: The bg_int number to issue when data is received on
 *              the extra interface and must be passed to BCHS
 *      intrFunc: Pointer to the bg_int function to call on reception of data
 *      pEI: Function pointer to probe the extra interface
 *      dEI: Function pointer to disconnect the extra interface
 *      sDEI: Function pointer to start the extra interface
 *      iEI: Function pointer to initialise the extra interface
 *      eEI: Function pointer to exit the extra interface
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetupUsb(uint8_t intrNumber, void (*intrFunc)(void),
              probeExtraInterface_t pEI,
              disconnectExtraInterface_t dEI,
              startDrvExtraInterface_t sDEI,
              initExtraInterface_t iEI,
              setupExtraInterface_t sEI,
              exitExtraInterface_t eEI);

/* Hooks to change the default USB settings */
void SetSupportedUsbDevices(struct usb_device_id *devs);
void SetUsbDriverName(char *name);
void SetUsbIsocAltSetting(int alt);
void SetUsbMaxIsocInFrames(int in);
void SetUsbMaxIsocOutFrames(int out);

/* Structure to hold the static settings (before the driver is initialized) */
typedef struct
{
    bool_t                       running_with_scheduler;
    bool_t                       ext_iface;
    probeExtraInterface_t        ext_probe;
    disconnectExtraInterface_t   ext_disconnect;
    startDrvExtraInterface_t     ext_start;
    initExtraInterface_t         ext_init;
    exitExtraInterface_t         ext_exit;
    int                          max_isoc_in_frames;
    int                          max_isoc_out_frames;
    int                          max_isoc_packet_size;
    int                          isoc_alt_setting;
    struct usb_device_id         *usb_devices;
    char                         *driver_name;
} csr_usb_settings_t;

#ifdef __cplusplus
}
#endif

#endif
