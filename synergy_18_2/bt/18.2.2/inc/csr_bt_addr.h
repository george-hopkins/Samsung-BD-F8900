#ifndef CSR_BT_ADDR_H__
#define CSR_BT_ADDR_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "tbdaddr.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Remap tbdaddr interface to Synergy compliant names.  This is to
 * allow for future changes without breaking the API */
typedef TYPED_BD_ADDR_T CsrBtTypedAddr;
typedef CsrUint8 CsrBtAddressType;

#define CSR_BT_ADDR_PUBLIC  TBDADDR_PUBLIC
#define CSR_BT_ADDR_PRIVATE TBDADDR_RANDOM
#define CSR_BT_ADDR_INVALID TBDADDR_INVALID

/* Subfields */
#define CsrBtAddrType(addrt) TBDADDR_TYPE((addrt))
#define CsrBtAddrAddr(addrt) TBDADDR_ADDR((addrt))
#define CsrBtAddrLap(addrt) TBDADDR_LAP((addrt))
#define CsrBtAddrUap(addrt) TBDADDR_UAP((addrt))
#define CsrBtAddrNap(addrt) TBDADDR_NAP((addrt))
#define CsrBtAddrInvalidate(addrt) TBDADDR_INVALIDATE((addrt))
#define CsrBtAddrZero(addrt) CsrMemSet((addrt), 0, sizeof(CsrBtTypedAddr))

/* Tests */
#define CsrBtAddrIsPublic(addrt) TBDADDR_IS_PUBLIC((addrt))
#define CsrBtAddrIsRandom(addrt) TBDADDR_IS_RANDOM((addrt))
#define CsrBtAddrIsPrivate(addrt) TBDADDR_IS_PRIVATE((addrt)) 
#define CsrBtAddrIsStatic(addrt) tbdaddr_is_static((addrt))
#define CsrBtAddrIsValid(addrt) tbdaddr_is_valid((addrt))
#define CsrBtAddrIsPrivateResolvable(addrt) tbdaddr_is_private_resolvable((addrt))
#define CsrBtAddrIsPrivateNonresolvable(addrt) tbdaddr_is_private_nonresolvable((addrt))
#define CsrBtAddrIsZero(addrt) (CsrBtBdAddrEqZero((addrt)) && CsrBtAddrIsPublic((addrt)))

/* Compare */
#define CsrBtAddrEq(a,b) tbdaddr_eq((a), (b))
#define CsrBtAddrEqWithType(addrt,type,bd) tbdaddr_eq_bd_addr((addrt), (type), (bd))

/* Copy */
#define CsrBtAddrCopy(dst,src) tbdaddr_copy((dst), (src))
#define CsrBtAddrCopyWithType(dst,type,src) tbdaddr_copy_from_bd_addr((dst), (type), (src))
#define CsrBtAddrCopyFromPublic(dst,src) tbdaddr_copy_from_public_bd_addr((dst), (src))
#define CsrBtAddrCopyToPublic(dst,src) tbdaddr_copy_to_bd_addr((dst), (src))

#ifdef __cplusplus
}
#endif

#endif  /* CSR_BT_ADDR_H__ */
