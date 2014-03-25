#ifndef CSR_BT_AV_FRAGMENTATION_H__
#define CSR_BT_AV_FRAGMENTATION_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_av_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLEAR_ALL_FRAGMENTS        0

typedef struct fragmentElement_tag
{
    CsrUint16                    length;
    CsrUint8                     *packetPtr;
    struct fragmentElement_tag  *next;
} fragmentElement_t;

typedef struct fragmentHead_tag
{
    CsrBtConnId                 btConnId;
    CsrUint16                    totalLength;
    struct fragmentHead_tag     *next;
    fragmentElement_t           *fragmentPtr;
} fragmentHead_t;

void CsrBtAvAllocFragmentHead( fragmentHead_t **headPtr, CsrBtConnId btConnId);
void CsrBtAvFreeFragments( fragmentHead_t **headPtr, CsrBtConnId btConnId);
void CsrBtAvAddFragment( fragmentHead_t *headPtr, CsrBtConnId btConnId, CsrUint16 length, CsrUint8 *fragment);
CsrUint8 *CsrBtAvDefragment( fragmentHead_t **headPtr, CsrBtConnId btConnId, CsrUint16 *length, CsrUint8 *fragment);

#ifdef __cplusplus
}
#endif

#endif
