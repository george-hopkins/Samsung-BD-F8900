#ifndef CSR_BT_PB_OBEX_H__
#define CSR_BT_PB_OBEX_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_pas_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* == GLOBAL TYPES ========================================================== */
typedef CsrUint32    PbAppParFlag;

/* == GLOBAL DEFINES ======================================================== */
#define CSR_BT_PB_OBEX_FILTER_FLAG             ((PbAppParFlag)(0x00000001))
#define CSR_BT_PB_OBEX_FORMAT_FLAG             ((PbAppParFlag)(0x00000002))
#define CSR_BT_PB_OBEX_MAX_LIST_CNT_FLAG       ((PbAppParFlag)(0x00000004))
#define CSR_BT_PB_OBEX_ORDER_FLAG              ((PbAppParFlag)(0x00000008))
#define CSR_BT_PB_OBEX_SEARCHVAL_FLAG          ((PbAppParFlag)(0x00000010))
#define CSR_BT_PB_OBEX_SEARCHATT_FLAG          ((PbAppParFlag)(0x00000020))
#define CSR_BT_PB_OBEX_LISTSTARTOFFSET_FLAG    ((PbAppParFlag)(0x00000040))
#define CSR_BT_PB_OBEX_PBSIZE_FLAG             ((PbAppParFlag)(0x00000080))
#define CSR_BT_PB_OBEX_NEWMISSEDCALL_FLAG      ((PbAppParFlag)(0x00000100))

/* Header ids */
#define CSR_BT_OBEX_PB_ORDER_ID                (0x01)
#define CSR_BT_OBEX_PB_SEARCH_VAL_ID           (0x02)
#define CSR_BT_OBEX_PB_SEARCH_ATT_ID           (0x03)
#define CSR_BT_OBEX_PB_MAX_LIST_CNT_ID         (0x04)
#define CSR_BT_OBEX_PB_LIST_START_OFFSET_ID    (0x05)
#define CSR_BT_OBEX_PB_FILTER_ID               (0x06)
#define CSR_BT_OBEX_PB_FORMAT_ID               (0x07)
#define CSR_BT_OBEX_PB_PHONEBOOK_SIZE_ID       (0x08)
#define CSR_BT_OBEX_PB_NEW_MISSED_CALLS_ID     (0x09)
#define CSR_BT_OBEX_PB_LAST_ID                 (0xFF)


/* == GLOBAL PROTOTYPES =========================================================== */
extern CsrUint16 PbRetAppParaHeaderLen(PbAppParFlag flag);
extern void PbBuildAppHeader(CsrUint8 **ppBuf, CsrUint8 **ppAppTotalLen);
extern CsrUint32 CsrBtObexWcsLen(const CsrUint8 *string);
extern CsrUint8 *CsrBtObexWcsCpy(CsrUint8 *dest, const CsrUint8 *src);
extern CsrUint8 *CsrBtObexWcsCat(CsrUint8 *dest, const CsrUint8 *src);
extern CsrInt32 CsrBtObexRevIWcsNMatch(const CsrUint8 *src, const CsrUint8 *pattern, CsrInt32 cnt);
#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PB_OBEX_H__ */

