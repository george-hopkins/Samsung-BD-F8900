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

/* Default PSR files and paths */
#ifndef CSR_BOOTSTRAP_PSR_DIR
#define CSR_BOOTSTRAP_PSR_DIR "."
#endif
#define CSR_BT_BOOTSTRAP_DEFAULT_PLATFORM CSR_BOOTSTRAP_PSR_DIR "/ps-default.psr"

/* Table of build-ids and associated patch bundles. Note that some
 * patch bundles exist in different variants based on what chip
 * package (CSP/QFN/BGA) that is used */
typedef struct
{
    CsrUint16      id;
    CsrCharString *patch;
    CsrBool        bc7up;
} csrBootstrapIdPatchTable;
static const csrBootstrapIdPatchTable csrBootstrapTable[] =
{
    {1958, CSR_BOOTSTRAP_PSR_DIR "/pb-2r17_1958.psr", FALSE},          /* BC4-ROM A04 */
    {3164, CSR_BOOTSTRAP_PSR_DIR "/pb-14r2_3164.psr", FALSE},          /* BC4-ROM A06 */
    {4363, CSR_BOOTSTRAP_PSR_DIR "/pb-22r14_4363.psr", FALSE},         /* BC5-FM A12 */
    {4698, CSR_BOOTSTRAP_PSR_DIR "/pb-26r17_4698.psr", FALSE},         /* BC5-FM A14 */
    {4839, CSR_BOOTSTRAP_PSR_DIR "/pb-28r2_4839.psr", FALSE},          /* BC4-ROM A07 */
    {4841, CSR_BOOTSTRAP_PSR_DIR "/pb-27r20_4841.psr", FALSE},         /* BC6-ROM A04 */
    {5985, CSR_BOOTSTRAP_PSR_DIR "/pb-51r22_5985_csp0p5.psr", TRUE},   /* BC7820 A12, CSP-0.5 */
    {6247, CSR_BOOTSTRAP_PSR_DIR "/pb-53r7_6247_csp.psr", TRUE},       /* BC7820 A16/BC7830 A16, CSP */
    {5892, CSR_BOOTSTRAP_PSR_DIR "/pb-49r3_5892.psr", TRUE},           /* BC7830 A14 */
    {6817, CSR_BOOTSTRAP_PSR_DIR "/pb-90r1_6817.psr", TRUE},           /* CSR8810 A04 */
    {7557, CSR_BOOTSTRAP_PSR_DIR "/ps-gemini_a06.psr", TRUE},          /* CSR8811 - LPC Final Release Candidate */
    {7558, CSR_BOOTSTRAP_PSR_DIR "/pb-101r2_7558_uart.psr", TRUE},     /* CSR8811 A06 ROM */
};

/* Application developer implemented function. This function will
 * automatically get called by the bootstrap process to retrieve any
 * platform specific settings that can only be determined at run-time
 * - e.g. the Bluetooth device friendly name, frequency trim,
 * Bluetooth address, etc. */
void CsrBootstrapSpecific(void *bootContext,
    CsrUint16 buildId,
    CsrBool *bc7up)
{
    CsrCharString *patch = NULL;
    CsrUint16 i;

    for (i = 0; i < CSR_ARRAY_SIZE(csrBootstrapTable); i++)
    {
        if (csrBootstrapTable[i].id == buildId)
        {
            patch = csrBootstrapTable[i].patch;
            *bc7up = csrBootstrapTable[i].bc7up;
            break;
        }
    }

    if (patch)
    {
        /* Load patch bundle for this chip */
        CsrBootstrapParsePsrFile(bootContext, patch);
    }

    /* Load default platform settings */
    CsrBootstrapParsePsrFile(bootContext,
        CSR_BT_BOOTSTRAP_DEFAULT_PLATFORM);
}
