#ifndef CSR_BT_GATT_FREE_HANDCODED_H__
#define CSR_BT_GATT_FREE_HANDCODED_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXCLUDE_CSR_BT_GATT_MODULE

void CsrBtGattFreeHandcoded(void *message);

/* The following functions must be handcoded */
void CsrBtGattDbAccessWriteIndPrimFree(void *message);
void CsrBtGattDbAddReqPrimFree(void *message);
void CsrBtGattScanReqPrimFree(void *message);
void CsrBtGattWriteReqPrimFree(void *message);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_GATT_FREE_HANDCODED_H__ */

