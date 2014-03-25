#ifndef ENV_PRIM_H__
#define ENV_PRIM_H__
/****************************************************************************

                (c) CSR plc 2007

                All rights reserved

FILE:           env_prim.h

DESCRIPTION:    The primitives used for accessing communicating with the
                environment

REVISION:       $Revision: #1 $

****************************************************************************/

#include "sched/csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="EnvPrim_t" */
/* conversion_rule="ADD_UNDERSCORE_AND_UPPERCASE_T" */

typedef uint16_t EnvPrim_t;

#define ENV_CLEANUP_IND        ((EnvPrim_t) 0x0000)

typedef struct
{
    EnvPrim_t           type;
    uint16_t            phandle;
} ENV_CLEANUP_IND_T;

#ifdef __cplusplus
}
#endif

#endif
