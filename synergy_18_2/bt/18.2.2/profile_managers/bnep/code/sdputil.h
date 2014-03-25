#ifndef _SDP_UTIL_H_
#define _SDP_UTIL_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2008

                   (c) CSR plc 2008

                   All rights reserved

FILE:              sdp_util.h

DESCRIPTION:       SDP utility functions.



****************************************************************************/

#ifndef BUILD_FOR_HOST
#include "bt.h"
#else
#include "csr_types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef INSTALL_PAN_MODULE
/******************************************************************************

    function sdp_get_uuid  --  get a UUID

The UUID must have a length of 2, 4 or 16 octets and
be stored MSB first (i.e. big-endianly).
If the UUID can't be expressed as a 16-bit UUID, NO_UUID is returned
(note this value has been chosen to be unlikely to be a valid UUID).

Currently only used within bnep and thus guarded by INSTALL_PAN_MODULE

******************************************************************************/

extern CsrUint16 sdp_get_uuid (const CsrUint8 *uuid, CsrUint16 uuidlen);

#define NO_UUID 0

#endif

#ifdef __cplusplus
}
#endif

#endif
