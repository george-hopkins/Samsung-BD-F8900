
/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_copy_from_bd_addr

\brief  Put an old-style BD_ADDR_T into a fat Bluetooth address.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"

void tbdaddr_copy_from_bd_addr(TYPED_BD_ADDR_T *dst,
                               CsrUint8 src_type,
                               const BD_ADDR_T *src)
{
    dst->type = src_type;
    dst->addr = *src;
}
