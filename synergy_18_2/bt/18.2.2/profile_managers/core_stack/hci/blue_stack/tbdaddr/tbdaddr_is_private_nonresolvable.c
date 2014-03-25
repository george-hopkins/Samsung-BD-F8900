/*!
        (c) CSR plc 2010
        All rights reserved

\file   tbdaddr_is_private_nonresolvable.c

\brief  Check type of fat Bluetooth address.
*/

#include "csr_synergy.h"

#include "tbdaddr_private.h"

CsrBool tbdaddr_is_private_nonresolvable(const TYPED_BD_ADDR_T *addrt)
{
    return TBDADDR_IS_RANDOM(*addrt)
            && (TBDADDR_NAP(*addrt) & TBDADDR_RANDOM_NAP_TYPE_MASK)
                    == TBDADDR_RANDOM_NAP_TYPE_PRIVATE_NONRESOLVABLE;
}
