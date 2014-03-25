#ifndef CSR_APP_MAIN_H__
#define CSR_APP_MAIN_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/* This function must be supplied by the application. It is called before the
   scheduler is started to allow the application to perform any required
   initialisation or argument parsing. */
CsrResult CsrAppMain(void);

/* This function must be supplied by the application and will be called if the
   --help argument is given, allowing the application may print additional usage
   information. */
void CsrAppMainUsage(void);

/* The following functions can be called by the application to retrieve
   Bluecore related parameters that have been extracted from the
   command line arguments. */
CsrUint16 CsrAppMainBluecoreTransportGet(void);
CsrUint32 CsrAppMainBluecorePreBootstrapBaudrateGet(void);
CsrUint32 CsrAppMainBluecorePostBootstrapBaudrateGet(void);
CsrUint32 CsrAppMainBluecoreXtalFrequencyGet(void);
void CsrAppMainBluecoreLocalDeviceBluetoothAddressGet(CsrUint8 address[6]);
const CsrCharString *CsrAppMainBluecoreLocalDeviceNameGet(void);
const CsrCharString *CsrAppMainBluecorePsrFileGet(void);
const CsrCharString *CsrAppMainBluecorePsrStringGet(void);

#ifdef __cplusplus
}
#endif

#endif
