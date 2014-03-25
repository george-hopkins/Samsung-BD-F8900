#ifndef CSR_BOOTSTRAP_GENERIC_H__
#define CSR_BOOTSTRAP_GENERIC_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

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

#ifdef __cplusplus
extern "C" {
#endif

/* Specific bootstrap helper function: Set the host transport
 * interface. Use the framework TRANSPORT_TYPE defines */
void CsrBootstrapSetInterface(void *bootContext,
    CsrUint16 hostInterface);

/* Specific bootstrap helper function: Set the host interface bitrate
 * (for use with UART host transports like BCSP, H4, H4DS etc) */
void CsrBootstrapSetBitRate(void *bootContext,
    CsrUint32 bitRate);

/* Specific bootstrap helper function: The the Bluetooth friendly
 * name */
void CsrBootstrapSetFriendlyName(void *bootContext,
    const CsrUtf8String *name);

/* Specific bootstrap helper function: Set the BlueCore Bluetooth
 * address */
void CsrBootstrapSetLocalBtAddress(void *bootContext,
    const CsrUint8 address[6]);

/* Specific bootstrap helper function: Set the crystal frequency */
void CsrBootstrapSetFrequency(void *bootContext,
    CsrUint16 frequency);

/* Specific bootstrap helper function: Set the crystal frequency
 * trim */
void CsrBootstrapSetFreqTrim(void *bootContext,
    CsrUint16 freqTrim);

/* Parse a PSR file and apply the settings from the file */
void CsrBootstrapParsePsrFile(void *bootContext, const CsrCharString *filename);

/* Parse a compiled-in constant C string and apply the settings */
void CsrBootstrapParsePsrConstant(void *bootContext, const CsrCharString *string);

/* Add a raw PS-key */
void CsrBootstrapAddPskey(void *bootContext, CsrUint16 key, CsrUint16 *data, CsrUint16 length);

#ifdef __cplusplus
}
#endif

#endif
