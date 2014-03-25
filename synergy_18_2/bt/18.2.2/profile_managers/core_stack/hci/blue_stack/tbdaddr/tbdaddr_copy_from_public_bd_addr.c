/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_from_pulic_bd_addr

\brief  Put an old-style public BD_ADDR_T into a fat Bluetooth address.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"

void tbdaddr_copy_from_public_bd_addr(TYPED_BD_ADDR_T *dst,
                                      const BD_ADDR_T *src)
{
    TBDADDR_TYPE(*dst) = TBDADDR_PUBLIC;
    TBDADDR_ADDR(*dst) = *src;
}
