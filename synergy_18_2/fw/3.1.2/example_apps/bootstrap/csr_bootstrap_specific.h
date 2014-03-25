#ifndef CSR_BOOTSTRAP_SPECIFIC_H__
#define CSR_BOOTSTRAP_SPECIFIC_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Application developer implemented function. This file is to be
 * implemented by the application developer. An example can be found
 * in the csr_bootstrap_specific_*.c files.  The application must set the
 * 'bc7up' variable to indicate whether the chip is a BC7 or newer in
 * order to set the host interface bitrate correctly */
extern void CsrBootstrapSpecific(void *bootContext,
    CsrUint16 buildId,
    CsrBool *bc7up);                                /* is chip BC7 or newer? */

#ifdef __cplusplus
}
#endif

#endif
