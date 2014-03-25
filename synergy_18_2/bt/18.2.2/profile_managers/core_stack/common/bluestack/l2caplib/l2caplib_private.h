/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_private.h

\brief  l2caplib private header file.
*/

#ifndef _L2CAPLIBPRIVATE_H_
#define _L2CAPLIBPRIVATE_H_

#include "csr_synergy.h"

#include "l2caplib.h"
#include "csr_mblk.h"
#include "tbdaddr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* While any users of L2CAP are not yet MBLK-enabled, we need to wrap
   any incoming payload data pointers into a MBLK to please L2CAP.

   Note: This macro repeats 'data' in its expansion and so should be
   considered unsafe. */
#define L2CA_MblkWrap(data, length) \
    ((length) == 0 ? (data) : CsrMblkDataCreate((data), (length), TRUE))

#ifdef __cplusplus
}
#endif

#endif /* _L2CAPLIBPRIVATE_H_ */
