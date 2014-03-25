/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

/* IMPORTANT NOTICE: This file sets up basic and constant settings for
 * the CSR Synergy BlueCore bootstrap system.  You may need to tweak
 * the settings in this file to match your platform */
#include "csr_log_text.h"
#include "csr_macro.h"

#include "csr_bt_bootstrap_generic.h"
#include "csr_bt_bootstrap_specific.h"

//bootstrap_patch_string_pb_101_7558_samsung
//bootstrap_patch_string_pb_90_6817_Samsung
//6817
//7558
/* Bootstrap strings (implemented in bootstrap/psr/pb-...c) */
extern const CsrCharString bootstrap_patch_string_pb_101_7558_samsung[];
extern const CsrCharString bootstrap_patch_string_pb_90_6817_Samsung_psr[];


/* Table of build-ids and associated patch bundles. Note that some
 * patch bundles exist in different variants based on what chip
 * package (CSP/QFN/BGA) that is used */
typedef struct
{
    CsrUint16 id;
    const CsrCharString *patch;
    CsrBool bc7up;
} csrBtBootstrapIdPatchTable;
static const csrBtBootstrapIdPatchTable csrBtBootstrapTable[] =
{
    { 6817, bootstrap_patch_string_pb_90_6817_Samsung_psr,                TRUE },/*8810*/ 
    { 7558, bootstrap_patch_string_pb_101_7558_samsung,               TRUE },/*7510*/

}; 


/* Application developer implemented function. This function will
 * automatically get called by the bootstrap process to retrieve any
 * platform specific settings that can only be determined at run-time
 * - e.g. the Bluetooth device friendly name, frequency trim,
 * Bluetooth address, etc. */

void CsrBtBootstrapSpecific(void *bootContext,
                            CsrUint16 buildId,
                            CsrBool *bc7up)
{
    const CsrCharString *patch = NULL;
    CsrUint16 i;

    for(i=0; i<CSR_ARRAY_SIZE(csrBtBootstrapTable); i++)
    {
        if(csrBtBootstrapTable[i].id == buildId)
        {
            patch = csrBtBootstrapTable[i].patch;
            *bc7up = csrBtBootstrapTable[i].bc7up;
            CsrBtBootstrapParsePsrConstant(bootContext, patch);
			break;
        }
    }
}

