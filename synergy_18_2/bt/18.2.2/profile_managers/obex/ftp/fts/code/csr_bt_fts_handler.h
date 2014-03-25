#ifndef CSR_BT_FTS_HANDLER_H__
#define CSR_BT_FTS_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_fts_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrUint8             localServerCh;
    psm_t                localPsm;
    void                *obexInst;
    dm_security_level_t  secIncoming; 
    CsrSchedQid          appHandle;
    CsrBool              finalFlag;
    CsrBool              typeHeaderFlag;
    CsrUint8             obexCommand;
    CsrUint8            *name;
    CsrUint16            nameLength;
    CsrBool              localSrmEnable;
    CsrUint16            localMaxPacketSize;
    CsrUint16            windowSize;
} FtsInstanceDataType;

typedef CsrUint8 (* FtsStateHandlerType)(FtsInstanceDataType * ftsInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif

