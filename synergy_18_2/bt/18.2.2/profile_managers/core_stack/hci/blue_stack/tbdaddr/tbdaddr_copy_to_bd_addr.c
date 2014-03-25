
/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_copy_to_bd_addr

\brief  Extract an old-style BD_ADDR_T from a fat Bluetooth address.
        Note that BD_ADDR_T does not contain address type information.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"

CsrUint8 tbdaddr_copy_to_bd_addr(BD_ADDR_T *dst, const TYPED_BD_ADDR_T *src)
{
    *dst = TBDADDR_ADDR(*src);
    return TBDADDR_TYPE(*src);
}
