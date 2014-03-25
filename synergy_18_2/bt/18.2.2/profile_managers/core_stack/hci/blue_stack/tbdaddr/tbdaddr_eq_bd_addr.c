/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_eq_bd_addr.c

\brief  Compare a fat Bluetooth address with a thin one.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"

CsrBool tbdaddr_eq_bd_addr(const TYPED_BD_ADDR_T *addrt,
                               CsrUint8 bd_addr_type,
                               const BD_ADDR_T *bd_addr)
{
    TYPED_BD_ADDR_T temp;

    tbdaddr_copy_from_bd_addr(&temp, bd_addr_type, bd_addr);
    return tbdaddr_eq(addrt, &temp);
}
