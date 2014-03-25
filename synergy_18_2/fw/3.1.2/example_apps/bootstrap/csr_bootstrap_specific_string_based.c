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

/* Bootstrap strings (implemented in bootstrap/psr/pb-...c) */
extern const CsrCharString bootstrap_patch_string_pb_14r2_3164_psr[];
extern const CsrCharString bootstrap_patch_string_pb_22r14_4363_psr[];
extern const CsrCharString bootstrap_patch_string_pb_26r17_4698_psr[];
extern const CsrCharString bootstrap_patch_string_pb_28r2_4839_psr[];
extern const CsrCharString bootstrap_patch_string_pb_27r20_4841_psr[];
extern const CsrCharString bootstrap_patch_string_pb_2r17_1958_psr[];
extern const CsrCharString bootstrap_patch_string_pb_49r3_5892_psr[];
extern const CsrCharString bootstrap_patch_string_pb_51r22_5985_bga_psr[];
extern const CsrCharString bootstrap_patch_string_pb_51r22_5985_csp0p4_psr[];
extern const CsrCharString bootstrap_patch_string_pb_51r22_5985_csp0p5_psr[];
extern const CsrCharString bootstrap_patch_string_pb_53r7_6247_csp_psr[];
extern const CsrCharString bootstrap_patch_string_pb_53r7_6247_qfn_psr[];
extern const CsrCharString bootstrap_patch_string_pb_90r1_6817_psr[];
extern const CsrCharString bootstrap_patch_string_pb_101r2_7558_uart_psr[];
extern const CsrCharString bootstrap_patch_string_ps_default_psr[];
extern const CsrCharString bootstrap_patch_string_ps_ble_psr[];
extern const CsrCharString bootstrap_patch_string_ps_gemini_a06_psr[];

/* Table of build-ids and associated patch bundles. Note that some
 * patch bundles exist in different variants based on what chip
 * package (CSP/QFN/BGA) that is used */
typedef struct
{
    CsrUint16            id;
    const CsrCharString *patch;
    CsrBool              bc7up;
} csrBootstrapIdPatchTable;
static const csrBootstrapIdPatchTable csrBootstrapTable[] =
{
    {1958, bootstrap_patch_string_pb_2r17_1958_psr, FALSE},           /* BC4-ROM A04 */
    {3164, bootstrap_patch_string_pb_14r2_3164_psr, FALSE},           /* BC4-ROM A06 */
    {4363, bootstrap_patch_string_pb_22r14_4363_psr, FALSE},          /* BC5-FM A12 */
    {4698, bootstrap_patch_string_pb_26r17_4698_psr, FALSE},          /* BC5-FM A14 */
    {4839, bootstrap_patch_string_pb_28r2_4839_psr, FALSE},           /* BC4-ROM A07 */
    {4841, bootstrap_patch_string_pb_27r20_4841_psr, FALSE},          /* BC6-ROM A04 */
    {5985, bootstrap_patch_string_pb_51r22_5985_csp0p5_psr, TRUE},    /* BC7820 A12, CSP-0.5 */
    {6247, bootstrap_patch_string_pb_53r7_6247_csp_psr, TRUE},        /* BC7820 A16/BC7830 A16, CSP */
    {5892, bootstrap_patch_string_pb_49r3_5892_psr, TRUE},            /* BC7830 A14 */
    {6817, bootstrap_patch_string_pb_90r1_6817_psr, TRUE},            /* CSR8810 A04 */
    {7557, bootstrap_patch_string_ps_gemini_a06_psr, TRUE},           /* CSR8811 - LPC Final Release Candidate */
    {7558, bootstrap_patch_string_pb_101r2_7558_uart_psr, TRUE},      /* CSR8811 A06 ROM */
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
    const CsrCharString *patch = NULL;
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
        CsrBootstrapParsePsrConstant(bootContext, patch);
    }

    /* Load default platform settings */
    CsrBootstrapParsePsrConstant(bootContext, bootstrap_patch_string_ps_default_psr);
}
