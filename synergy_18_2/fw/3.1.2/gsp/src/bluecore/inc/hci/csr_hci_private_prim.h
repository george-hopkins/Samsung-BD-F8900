#ifndef CSR_HCI_PRIVATE_PRIM_H__
#define CSR_HCI_PRIVATE_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrHciPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

#define CSR_HCI_PRIVATE_PRIM (CSR_HCI_PRIM)

#define CSR_HCI_NOP_IND ((CsrHciPrim) ((0x0000 + CSR_HCI_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))

typedef struct
{
    CsrHciPrim  type;
    CsrSchedQid phandle;        /* Handle to send the forwarded NOP message to */
    CsrUint8   *payload;        /* Pointer to the forwarded NOP message */
    CsrUint16   payloadLength;  /* The length of the NOP message */
} CsrHciNopInd;

#ifdef __cplusplus
}
#endif


#endif /* CSR_HCI_PRIVATE_PRIM_H__ */
