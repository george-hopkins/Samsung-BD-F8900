/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_copy.c

\brief  Copy a fat Bluetooth device address.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"

void tbdaddr_copy(TYPED_BD_ADDR_T *dst, const TYPED_BD_ADDR_T *src)
{
    *dst = *src;
}
