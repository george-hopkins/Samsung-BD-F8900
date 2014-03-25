#ifndef CSR_BCCMD_PRIVATE_PRIM_H__
#define CSR_BCCMD_PRIVATE_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"
#include "csr_bccmd_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBccmdPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

#define CSR_BCCMD_PRIVATE_PRIM (CSR_BCCMD_PRIM)

/* Private Downstream primitives    */
#define CSR_BCCMD_BOOTSTRAP_REQ     ((CsrBccmdPrim) 0x0000 + CSR_BCCMD_PRIM_DOWNSTREAM_COUNT)
#define CSR_BCCMD_RESET_RES         ((CsrBccmdPrim) 0x0001 + CSR_BCCMD_PRIM_DOWNSTREAM_COUNT)
#define CSR_BCCMD_GET_BUILD_ID_REQ  ((CsrBccmdPrim) 0x0002 + CSR_BCCMD_PRIM_DOWNSTREAM_COUNT)
#define CSR_BCCMD_QUERY_REQ         ((CsrBccmdPrim) 0x0003 + CSR_BCCMD_PRIM_DOWNSTREAM_COUNT)
#define CSR_BCCMD_PANIC_ARGS_REQ    ((CsrBccmdPrim) 0x0004 + CSR_BCCMD_PRIM_DOWNSTREAM_COUNT)
#define NUM_OF_CSR_BCCMD_PRIVATE_DOWNSTREAM_PRIMS (0x05)
#define TOTAL_NUM_OF_CSR_BCCMD_DOWNSTREAM_PRIMS (CSR_BCCMD_PRIM_DOWNSTREAM_COUNT + NUM_OF_CSR_BCCMD_PRIVATE_DOWNSTREAM_PRIMS)

/* Private Upstream primitives      */
#define CSR_BCCMD_RESET_IND         ((CsrBccmdPrim) ((0x0000 + CSR_BCCMD_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define CSR_BCCMD_BOOTSTRAP_CFM     ((CsrBccmdPrim) ((0x0001 + CSR_BCCMD_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define CSR_BCCMD_GET_BUILD_ID_CFM  ((CsrBccmdPrim) ((0x0002 + CSR_BCCMD_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define CSR_BCCMD_PANIC_IND         ((CsrBccmdPrim) ((0x0003 + CSR_BCCMD_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define CSR_BCCMD_PANIC_ARGS_CFM    ((CsrBccmdPrim) ((0x0004 + CSR_BCCMD_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define NUM_OF_CSR_BCCMD_PRIVATE_UPSTREAM_PRIMS (0x05)
#define TOTAL_NUM_OF_CSR_BCCMD_UPSTREAM_PRIMS (CSR_BCCMD_PRIM_UPSTREAM_COUNT + NUM_OF_CSR_BCCMD_PRIVATE_UPSTREAM_PRIMS)

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrSchedQid  phandle;                     /* Handle to application                            */
    CsrUint16    payloadLength;              /* The total length of the BCCMD messages           */
    CsrUint8    *payload;                    /* Pointer to the BCCMD messages                    */
} CsrBccmdBootstrapReq;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
} CsrBccmdResetInd;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrResult    status;                /* Indicates whether the reset succeeded (CSR_RESULT_SUCCESS) */
} CsrBccmdResetRes;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrResult    status;               /* Status information about the BCCMD transaction   */
} CsrBccmdBootstrapCfm;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrSchedQid  phandle;                     /* Handle to application                            */
} CsrBccmdGetBuildIdReq;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrResult    status;               /* Status information about the BCCMD transaction   */
    CsrUint16    buildId;                    /* Identifies the BlueCore module's firmware version*/
} CsrBccmdGetBuildIdCfm;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier */
} CsrBccmdQueryReq;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
} CsrBccmdPanicInd;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrSchedQid  phandle;                   /* Handle to application                            */
} CsrBccmdPanicArgsReq;

typedef struct
{
    CsrBccmdPrim type;                      /* Event identifier                                 */
    CsrResult    panicStatus;               /* Status information about the BCCMD transaction   */
    CsrUint16    panicArg;
    CsrResult    faultStatus;
    CsrUint16    faultArg;
} CsrBccmdPanicArgsCfm;

#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_PRIVATE_PRIM_H__ */
