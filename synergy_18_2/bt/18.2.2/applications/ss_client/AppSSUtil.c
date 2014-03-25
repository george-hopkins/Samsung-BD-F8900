/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "bluetooth.h"
#include "csr_pmem.h"
#include "AppSSutil.h"

void ss_bd_addr_copy(BD_ADDR_T *p_bd_addr_dest, const BD_ADDR_T *p_bd_addr_src)
{
    CsrMemCpy(p_bd_addr_dest, p_bd_addr_src, sizeof(BD_ADDR_T));
}

/*! \brief Set the BD Address to all zeroes.

    \param p_bd_addr Pointer to Bluetooth address to be zeroed.
*/
void ss_bd_addr_zero(BD_ADDR_T *p_bd_addr)
{
    CsrMemSet(p_bd_addr, 0 , sizeof(BD_ADDR_T));
}

/*! \brief Compare two BD Addresses for equality.

    \param p_bd_addr_1 Pointer to first Bluetooth address.
    \param p_bd_addr_2 Pointer to second Bluetooth address.
    \returns TRUE if equal, FALSE if different.
*/
CsrBool ss_bd_addr_eq(
    const BD_ADDR_T *p_bd_addr_1,
    const BD_ADDR_T *p_bd_addr_2
    )
{
    if ((p_bd_addr_1->lap == p_bd_addr_2->lap) &&
        (p_bd_addr_1->uap == p_bd_addr_2->uap) &&
        (p_bd_addr_1->nap == p_bd_addr_2->nap))
    {
        return TRUE;
    }

    return FALSE;
}

