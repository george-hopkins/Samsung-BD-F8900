/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* IMPORTANT NOTICE: This file sets up basic and constant settings for
 * the CSR Synergy BlueCore bootstrap system.  You may need to tweak
 * the settings in this file to match your platform */

#include "csr_types.h"
#include "csr_bootstrap_generic.h"
#include "csr_bootstrap_specific.h"

#define CSR_BOOTSTRAP_MY_PATCH "pb-bc7fm.psr"
#define CSR_BOOTSTRAP_MY_SETTINGS "ps-handset.psr"

/* Example file for a custom platform bootstrap file */
void CsrBootstrapSpecific(void *bootContext,
    CsrUint16 buildId,
    CsrBool *bc7up)
{
    /* Load patch bundle for this chip */
    CsrBootstrapParsePsrFile(bootContext, CSR_BOOTSTRAP_MY_PATCH);

    /* Load default platform settings */
    CsrBootstrapParsePsrFile(bootContext, CSR_BOOTSTRAP_MY_SETTINGS);

    /* Don't care about the buildId - we know _exactly_ what chip
     * we're using in this project */
    CSR_UNUSED(buildId);
}
