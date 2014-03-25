#ifndef _CSR_BT_BOOT_GENERIC_H_
#define _CSR_BT_BOOT_GENERIC_H_

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

/* IMPORTANT NOTICE: This file contains generic functions for the CSR
 * Synegy BlueCore bootstrap system. You should NOT touch anything
 * here. Instead use the 'csr_bt_boot_specific.c/h' files to set up
 * the bootstrap for your particular system */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_tm_bluecore_bootstrap.h"
#include "csr_transport.h"
#include "csr_usr_config.h"
#include "csr_util.h"
#include "csr_bt_profiles.h"
#include "csr_bt_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Application developer implemented function. This file is to be
 * implemented by the application developer. An example can be found
 * in the csr_bt_boot_specific.c file.  The application must set the
 * 'bc7up' variable to indicate whether the chip is a BC7 or newer in
 * order to set the host interface bitrate correctly */
extern void CsrBtBootstrapSpecific(void *bootContext,
                                   CsrUint16 buildId,
                                   CsrBool *bc7up); /* is chip BC7 or newer? */

/* Specific bootstrap helper function: Set the host transport
 * interface. Use the framework TRANSPORT_TYPE defines */
void CsrBtBootstrapSetInterface(void *bootContext,
                                CsrUint16 hostInterface);

/* Specific bootstrap helper function: Set the host interface bitrate
 * (for use with UART host transports like BCSP, H4, H4DS etc) */
void CsrBtBootstrapSetBitRate(void *bootContext,
                              CsrUint32 bitRate);

/* Specific bootstrap helper function: The the Bluetooth friendly
 * name */
void CsrBtBootstrapSetFriendlyName(void *bootContext,
                                   CsrUtf8String *name);
                                    
/* Specific bootstrap helper function: Set the BlueCore Bluetooth
 * address */   
void CsrBtBootstrapSetLocalBtAddress(void *bootContext,
                                     CsrBtDeviceAddr *address);

/* Specific bootstrap helper function: Set the crystal frequency */
void CsrBtBootstrapSetFrequency(void *bootContext,
                                CsrUint16 frequency);

/* Specific bootstrap helper function: Set the crystal frequency
 * trim */
void CsrBtBootstrapSetFreqTrim(void *bootContext,
                               CsrUint16 freqTrim);

/* Parse a PSR file and apply the settings from the file */
void CsrBtBootstrapParsePsrFile(void *bootContext, const CsrCharString *filename);

/* Parse a compiled-in constant C string and apply the settings */
void CsrBtBootstrapParsePsrConstant(void *bootContext, const CsrCharString *string);

/* Add a raw PS-key */    
void CsrBtBootstrapAddPskey(void *bootContext, CsrUint16 key, CsrUint16 *data, CsrUint16 length);

#ifdef __cplusplus
}
#endif

#endif
