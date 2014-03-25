/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_util.h"
#include "csr_msgconv.h"
#include "csr_bt_converter_utils.h"

void CsrBtConverterSerialiseBtTypedAddr(CsrUint8 * ptr, CsrSize * length, CsrBtTypedAddr *source)
{
    CsrUint8Ser(ptr, length, (CsrUint8)source->type);
    CsrUint16Ser(ptr, length, (CsrUint16)source->addr.nap);
    CsrUint8Ser(ptr, length, (CsrUint8)source->addr.uap);
    CsrUint32Ser(ptr, length, (CsrUint32)source->addr.lap);
}

void CsrBtConverterDeserialiseBtTypedAddr(CsrUint8 *buffer, CsrSize *offset, CsrBtTypedAddr *target)
{
    CsrUint8Des((CsrUint8 *)&(target->type), buffer, offset);
    CsrUint16Des((CsrUint16 *)&(target->addr.nap), buffer, offset);
    CsrUint8Des((CsrUint8 *)&(target->addr.uap), buffer, offset);
    CsrUint32Des((CsrUint32 *)&(target->addr.lap), buffer, offset);
}
