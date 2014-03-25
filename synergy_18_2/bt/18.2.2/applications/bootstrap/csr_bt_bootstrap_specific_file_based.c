/*****************************************************************************

        Copyright Cambridge Silicon Radio Limited and its licensors 2011.
        All rights reserved.

*****************************************************************************/

#include "csr_synergy.h"

/* IMPORTANT NOTICE: This file sets up basic and constant settings for
 * the CSR Synergy BlueCore bootstrap system.  You may need to tweak
 * the settings in this file to match your platform */

#include "csr_bt_bootstrap_generic.h"
#include "csr_bt_bootstrap_specific.h"

/* Default PSR files and paths */
#define _PSRPATH_ CSR_BT_TOPDIR"/applications/bootstrap/psr/"
#define CSR_BT_BOOTSTRAP_DEFAULT_PLATFORM _PSRPATH_"ps-default.psr"

/* Table of build-ids and associated patch bundles. Note that some
 * patch bundles exist in different variants based on what chip
 * package (CSP/QFN/BGA) that is used */
typedef struct
{
    CsrUint16 id;
    CsrCharString *patch;
    CsrBool bc7up;
} csrBtBootstrapIdPatchTable;
static const csrBtBootstrapIdPatchTable csrBtBootstrapTable[] =
{
    { 1958, _PSRPATH_"pb-2r17_1958.psr",                FALSE }, /* BC4-ROM A04 */
    { 3164, _PSRPATH_"pb-14r2_3164.psr",                FALSE }, /* BC4-ROM A06 */
    { 4363, _PSRPATH_"pb-22r14_4363.psr",               FALSE }, /* BC5-FM A12 */
    { 4698, _PSRPATH_"pb-26r17_4698.psr",               FALSE }, /* BC5-FM A14 */
    { 4839, _PSRPATH_"pb-28r2_4839.psr",                FALSE }, /* BC4-ROM A07 */
    { 4841, _PSRPATH_"pb-27r26_4841.psr",               FALSE }, /* BC6-ROM A04 */
    { 6311, _PSRPATH_"pb-74r3_6311.psr",                FALSE }, /* BC6-ROM A05 */
    { 5985, _PSRPATH_"pb-51r22_5985_csp0p5.psr",        TRUE  }, /* BC7820 A12, CSP-0.5 */
    { 6247, _PSRPATH_"pb-53r11_6247_csp.psr",           TRUE  }, /* BC7820 A16/BC7830 A16, CSP */
    { 5892, _PSRPATH_"pb-49r3_5892.psr",                TRUE  }, /* BC7830 A14 */
    { 6817, _PSRPATH_"pb-90r1_6817.psr",                TRUE  }, /* CSR8810 A04 */
    { 7557, _PSRPATH_"pb-101r10_7558_csr8810_uart.psr", TRUE  }, /* CSR8811 - LPC Final Release Candidate */
    { 7558, _PSRPATH_"pb-101r10_7558_csr8810_uart.psr", TRUE  }, /* CSR8811 A06 ROM */
    { 7727, _PSRPATH_"ps-gemini_a06.psr",               TRUE  }, /* CSR8811 [LPC pre-A08] */
    { 7837, _PSRPATH_"ps-gemini_a06.psr",               TRUE  }, /* CSR8505 with LPC [LPC pre-A08] */
    { 8211, _PSRPATH_"ps-gemini_a06.psr",               TRUE  }, /* CSR8505 with LPC [LPC pre-A08] RC7 */
    { 8231, _PSRPATH_"ps-gemini_a06.psr",               TRUE  }, /* CSR8505 with LPC [LPC pre-A08] RC9 */
    { 8129, _PSRPATH_"ps-gemini_a06.psr",               TRUE  }, /* CSR8505 with LPC B-98367 */
    { 8241, _PSRPATH_"ps-gemini_a06.psr",               TRUE  }, /* CSR8505 with LPC [LPC pre-A08] RC9 */
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
    CsrCharString *patch = NULL;
    CsrUint16 i;

    for(i=0; i<CSR_ARRAY_SIZE(csrBtBootstrapTable); i++)
    {
        if(csrBtBootstrapTable[i].id == buildId)
        {
            patch = csrBtBootstrapTable[i].patch;
            *bc7up = csrBtBootstrapTable[i].bc7up;
            CsrBtBootstrapParsePsrFile(bootContext, patch);
        }
    }


#if defined(CSR_AMP_ENABLE)
#if defined(CSR_WIFI_DESIGN_M2107_R02) || defined(CSR_WIFI_DESIGN_M2107_R03)
    CsrBtBootstrapParsePsrFile(bootContext, _PSRPATH_"ps-jem_a12.psr");
    CsrBtBootstrapParsePsrFile(bootContext, _PSRPATH_"ps-m2107_coex.psr");
#endif
#if defined(CSR_WIFI_DESIGN_M2399_R3)
    CsrBtBootstrapParsePsrFile(bootContext, _PSRPATH_"ps-m2399_coex.psr");
#endif
#endif /* CSR_AMP_ENABLE */

    /* Load default platform settings */
    CsrBtBootstrapParsePsrFile(bootContext, 
                               CSR_BT_BOOTSTRAP_DEFAULT_PLATFORM);
}
