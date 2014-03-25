/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_main.h"
#include "csr_arg_search.h"

int main(int argc, char *argv[])
{
    CsrArgSearchInit((CsrUint32) argc, (CsrCharString **) argv);
    return (int) CsrMain();
}
