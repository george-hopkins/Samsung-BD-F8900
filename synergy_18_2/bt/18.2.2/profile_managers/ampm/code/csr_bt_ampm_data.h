#ifndef CSR_BT_AMPM_DATA_H
#define CSR_BT_AMPM_DATA_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Helper macroes */
#define CSR_BT_AMPM_L2CA_TO_AMP_ID(amp_id)                    (CsrUint8)(amp_id)
#define CSR_BT_AMPM_AMP_TO_L2CA_ID(amp_id)                    (CsrUint16)(amp_id)
#define CSR_BT_AMPM_IS_VALID_TYPE(ampType) \
    ((CSR_BT_AMP_AMP_CONTROLLER_TYPE_BR_EDR == (ampType)) || \
     (CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 == (ampType)) || \
     (CSR_BT_AMP_AMP_CONTROLLER_TYPE_FAKE == (ampType)))

/* Data format sent over the air */
typedef struct
{
    CsrBtAmpmId id;
    CsrBtAmpmType type;
    CsrUint8 status;
} CsrBtAmpmDataRemoteAmp;

typedef struct
{
    CsrUint16 length;
    void *data;
} CsrBtAmpmAmpAssoc;

typedef struct
{
    CsrBtAmpmId id;
    CsrBtAmpmType type;
    CsrUint8 status;
    CsrUint32 totalBandwidth;
    CsrUint32 guaranteedBandwidth;
    CsrUint32 minLatency;
    CsrUint32 maxPDUSize;
    CsrUint16 palCapabilities;
    CsrUint16 maxHciAssocSize;
    CsrBtAmpmAmpAssoc *pAMPAssoc;
    CsrUint32 maxFlushTimeout;
    CsrUint32 bestEffortFlushTimeout;
} CsrBtAmpmDataInfo;

typedef struct
{
    CsrUint8  keyType;
    CsrUint8  keySize;
    CsrUint8 *key;
} CsrBtAmpmDataAmpKey;

typedef struct
{
    CsrBtAmpmGenListMember header;
    BD_ADDR_T bd_addr;
    CsrUint8 sspKey[SIZE_LINK_KEY];
    CsrUint8 sspKeyType;
    CsrUint8 gampKey[CSR_BT_AMP_LINK_KEY_SIZE_GAMP];
    CsrUint8 gampKeyType;
    CsrBtAmpmDataAmpKey ampKeys[CSR_BT_AMPM_MAX_LOCAL_AMPS];
} CsrBtAmpmDataDeviceKeys;

/* Performs general initialisation */
extern void CsrBtAmpmDataInit(CsrBtAmpmInstance * inst);

/* Performs general deinitialisation */
extern void CsrBtAmpmDataDeinit(CsrBtAmpmInstance * inst);

/* Processes messages from the SC (stored link keys) */
extern void CsrBtAmpmDataScHandler(CsrBtAmpmInstance *inst);

/* Trigger read of stored link keys in the SC (sc_db) */
extern void CsrBtAmpmDataScTrigger(CsrBtAmpmInstance *inst,
                                   BD_ADDR_T *addr);

/* Sets the AMP Manager timer state to that specified */
extern void CsrBtAmpmDataSetTimersEnableState(CsrBtAmpmInstance * inst,
                                              CsrBool enabledState);

/* Will free the specified keys associated with a BT Device */
extern void CsrBtAmpmDataKeysDelete(CsrBtAmpmInstance * inst,
                                    CsrBtAmpmDataDeviceKeys * pBtDeviceKeys);


/* Will handle a received DM AMPM AMP KEY NOTIFICATION IND either
 * recording the new key or update an existing key. */
extern void CsrBtAmpmDataDmAmpmKeyNotificationInd(CsrBtAmpmInstance * inst,
                                                  DM_AMPM_LINK_KEY_NOTIFICATION_IND_T * prim);


/* Return the appropriate AMP key - newly generated or reused. */
extern CsrBtAmpmDataAmpKey *CsrBtAmpmDataKeysGetKeyForAmp(CsrBtAmpmInstance * inst,
                                                          BD_ADDR_T bd_addr,
                                                          CsrBtAmpmType Type);

/* Sets the local BT addr to the specified value */
extern void CsrBtAmpmDataSetLocalBtAddr(CsrBtAmpmInstance * inst,
                                        BD_ADDR_T bd_addr);

/* Returns the local BT addr to the specified value */
extern BD_ADDR_T CsrBtAmpmDataGetLocalBtAddr(CsrBtAmpmInstance * inst);

/* Handles the request to enable or disable the AMP SDU ignore
 * behaviour. */
extern void CsrBtAmpmDataSetIgnoreSduOptions(CsrBtAmpmInstance * inst,
                                             CsrBool EnableState,
                                             CsrUint8 SduId,
                                             CsrUint8 Count);

/* Returns whether the supplied AMP SDU id should be ignored or
 * handled. */
extern CsrBool CsrBtAmpmDataIgnoreSdu(CsrBtAmpmInstance * inst,
                                      CsrUint8 SduId);

/* Handles the request to enable or disable the change key for a
 * specified amp type ignore behaviour. */
extern void CsrBtAmpmDataSetChangeAmpKeyOptions(CsrBtAmpmInstance * inst,
                                                CsrBool EnableState,
                                                CsrBtAmpmType ampType,
                                                CsrUint8 Count);

/* Returns whether the supplied AMP types key should be altered from
 * the real one when send the HCI CREATE PHYSICAL LINK CMD */
extern CsrBool CsrBtAmpmDataChangeAmpKey(CsrBtAmpmInstance * inst,
                                         CsrBtAmpmType SduId);


/* Handles the request to set the whether received SDUs are passed out
 * via the AMPM SAP. */
extern void CsrBtAmpmDataSetRxSduControlOption(CsrBtAmpmInstance *inst,
                                               CsrBtAmpmControl RxSduControl,
                                               CsrSchedQid qid);

/* Returns whether received SDUs should be copied to the AMPM SAP or
 * not. */
extern CsrBool CsrBtAmpmL2capCopyRxSduToAmpmSap(CsrBtAmpmInstance *inst);

/* Returns whether HCI event forwarding is on */
extern CsrBool CsrBtAmpmSendRxHciToTester(CsrBtAmpmInstance * inst);

/* Returns whether received SDUs should be sent to the AMPM SAP and
 * not handled within the AMP Manager or not. */
extern CsrBool CsrBtAmpmSendRxSduToAmpmSap(CsrBtAmpmInstance *inst);

/* Send test command: get amp assoc request */
extern void CsrBtAmpmSendTestGetAssocReq(CsrBtAmpmInstance *inst,
                                         CsrBtDeviceAddr *addr,
                                         CsrUint8 signalId,
                                         CsrBtAmpmId ampId);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_DATA_H */
