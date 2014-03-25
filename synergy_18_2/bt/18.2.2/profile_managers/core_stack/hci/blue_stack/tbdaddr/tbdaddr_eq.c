/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_copy.c

\brief  Copy a fat Bluetooth device address.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"
#include "csr_bt_common.h"

CsrBool tbdaddr_eq(const TYPED_BD_ADDR_T *a, const TYPED_BD_ADDR_T *b)
{
    return TBDADDR_TYPE(*a) == TBDADDR_TYPE(*b)
                && bd_addr_eq(&TBDADDR_ADDR(*a), &TBDADDR_ADDR(*b));
}
