/****************************************************************************

                (c) Cambridge Silicon Radio Limited 2009

                All rights reserved and confidential information of CSR

***************************************************************************/

#include "csr_synergy.h"

#ifndef CSR_BT_UCS2HELPER_H__
#define CSR_BT_UCS2HELPER_H__

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_unicode.h"
#include "csr_bt_util.h"


#ifdef __cplusplus
extern "C" {
#endif

CsrSize CsrConverterUcs2ByteStrLen(const CsrUcs2String *str)
{
    return CsrUcs2ByteStrLen(str);
}

#ifdef __cplusplus
}
#endif

#endif
