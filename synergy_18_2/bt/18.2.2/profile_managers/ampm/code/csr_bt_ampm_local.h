#ifndef CSR_BT_AMPM_LOCAL_H
#define CSR_BT_AMPM_LOCAL_H

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrCmnListDataElm_t *next; /* Do not move - used for common linked list handling */
    CsrCmnListDataElm_t *prev; /* Do not move - used for common linked list handling */
    fsm_state_t state;
    CsrBool initialised;
    CsrSchedQid dataQueue;
    CsrSchedQid controlQueue;
    CsrBtAmpmDataInfo *pAmpInfo;
    CsrUint8 physicalLinkHandle;
    CsrUint16 assocExpectedLength;
    CsrUint16 assocLengthRemaining;
    CsrUint16 assocLengthSoFar;
    CsrUint8 *ampAssoc;
    CsrUint16 assocInsertOffset;
    CsrUint8 writeRemoteAssocPhysicalLinkHandle;
    CsrUint16 assocReadOffset;
    CsrSchedTid responseTid;
    CstBtAmpmGenListAnchor *pHciCmdQueueAnchor;
    CstBtAmpmGenListAnchor *pReadAmpAssocQueueAnchor;
} CsrBtAmpmLocalAmp;

/* Performs general initialisation associated with the Local AMP
 * functions within AMP Manager. Should be called once at start up */
extern void CsrBtAmpmLocalInit(CsrBtAmpmInstance *inst);

/* Performs general deinitialisation associated with the Local AMP
 * functions within AMP Manager. Should be called once at shutdown. */
extern void CsrBtAmpmLocalDeinit(CsrBtAmpmInstance *inst);


/* Attempts to create a new Local AMP instance. */
extern CsrBtAmpmLocalAmp *CsrBtAmpmLocalNew(CsrBtAmpmInstance * inst,
                                            CsrBtAmpmId id,
                                            CsrBtAmpmType type,
                                            CsrSchedQid dataQid,
                                            CsrSchedQid controlQid,
                                            CsrUint32 flags);


/* Deletes the specified Local AMP instance (and any assocated
 * data). */
extern void CsrBtAmpmLocalDelete(CsrBtAmpmInstance *inst,
                                 CsrBtAmpmLocalAmp *pLocalAmp);


/* Injects a NOP command complete into the FSM to */
extern void CsrBtAmpmLocalHciNop(CsrBtAmpmInstance *inst,
                                 CsrBtAmpmLocalAmp *pLocalAmp);


/* Informs the local amp instance of the AMP status changing */
extern void CsrBtAmpmLocalStatusChange(CsrBtAmpmInstance *inst,
                                       CsrBtAmpmId id,
                                       CsrBtAmpAmpStatus ampStatus);


/* Returns the Id of the specified local Amp */
extern CsrBtAmpmId CsrBtAmpmLocalGetId(const CsrBtAmpmLocalAmp *pLocalAmp);


/* Returns the type of the specified local Amp */
extern CsrBtAmpmType CsrBtAmpmLocalGetType(const CsrBtAmpmLocalAmp *pLocalAmp);


/* Returns the status of the specified local Amp */
extern CsrUint8 CsrBtAmpmLocalGetStatus(const CsrBtAmpmLocalAmp *pLocalAmp);


/* Attempts to find a local AMP instance that matches the specified
 * AMP Id */
extern CsrBtAmpmLocalAmp *CsrBtAmpmLocalGetById(CsrBtAmpmInstance *inst,
                                                CsrBtAmpmId Id);

/* Find the first unused AMP ID and return it */
extern CsrBtAmpmId CsrBtAmpmLocalGetNewId(CsrBtAmpmInstance *inst);

/* Attempts to find a local AMP instance that matches the specified
 * AMP type */
extern CsrBtAmpmLocalAmp *CsrBtAmpmLocalGetByType(CsrBtAmpmInstance *inst,
                                                  CsrBtAmpmType Type);

/* Returns the largest MTU Size supported by the available AMPs. */
extern CsrUint16 CsrBtAmpmLocalMaxMtuSupportedbyAmps(CsrBtAmpmInstance *inst);

/* Will return whether the specified local AMP can support the
 * specified QoS or not. */
extern CsrBool CsrBtAmpmLocalCanSupportQoS(CsrBtAmpmInstance *inst,
                                           CsrBtAmpmLocalAmp *pLocalAmp,
                                           CsrUint8 RequiredServiceType,
                                           CsrUint32 RequiredBandwidth,
                                           CsrUint32 RequiredAccessLatency);

/* ill return whether a local AMP can support the specified oS or
 * not. */
extern CsrBool CsrBtAmpmLocalAnAmpCanSupportQoS(CsrBtAmpmInstance *inst,
                                                CsrUint8 RequiredServiceType,
                                                CsrUint32 RequiredBandwidth,
                                                CsrUint32 RequiredAccessLatency);

/* Will attempt to update the supplied flowspecs with the best
 * available guaranteed QoS. */
extern void CsrBtAmpmLocalUpdateWithBestQoS(CsrBtAmpmInstance *inst,
                                            BD_ADDR_T bd_addr,
                                            L2CA_FLOWSPEC_T *pTxFlowSpec,
                                            L2CA_FLOWSPEC_T *pRxFlowSpec);

/* eturns the maximum PDU size supported by the local AMP. */
extern CsrUint16 CsrBtAmpmLocalGetMaxPduSize(CsrBtAmpmLocalAmp *pLocalAmp);

/* eturns the total bandwidth size supported by the local AMP. */
extern CsrUint32 CsrBtAmpmLocalGetMaxBandwidth(CsrBtAmpmLocalAmp *pLocalAmp);

/* Returns the best effort flushtimeout supported by the local AMP. */
extern CsrUint32 CsrBtAmpmLocalGetBestEffortFlushTimeout(CsrBtAmpmLocalAmp *pLocalAmp);

/* Requests the local AMP instance to obtain the local amp info */
extern void CsrBtAmpmLocalReadAmpInfo(CsrBtAmpmBtdev *pRequester,
                                      CsrUint8 SduIdentifier,
                                      CsrBtAmpmLocalAmp *pLocalAmp);

/* Requests the local AMP instance to obtain the local amp assoc */
extern void CsrBtAmpmLocalReadAmpAssoc(CsrBtAmpmInstance *inst,
                                       void *pRequester,
                                       CsrBtAmpmLocalAmp *pLocalAmp,
                                       CsrUint8 SduIdentifier,
                                       CsrUint8 PhysicalLinkHandle,
                                       fsm_event_t fsmEvent);

/* Requests the local AMP instance to write the supplied remote amp
 * assoc */
extern CsrBool CsrBtAmpmLocalWriteRemoteAmpAssoc(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLocalAmp *pLocalAmp,
                                                 CsrUint8 PhysicalLinkHandle,
                                                 CsrBtAmpmAmpAssoc *pAssoc);

/* Function for handling a response timeout and ensuring that any
 * blocking HCI command in removed from the required Local AMP
 * transmission queue. */
extern void CsrBtAmpmLocalHciQueueRemovalTimeoutInd(CsrUint16 ignored,
                                                    void *argp);

/* Function for handling a response timeout and signalling the event
 * into the supplied local amp FSM instance (if it is still valid). */
extern void CsrBtAmpmLocalHciResponseTimeoutInd(CsrUint16 ignored,
                                                void *argp);

/* Function for handling the timeout that signals that the AMP
 * instance if still not initialised should attempt to read the local
 * amp info again. */
extern void CsrBtAmpmLocalReadLocalAmpInfoTimeoutInd(CsrUint16 ignored,
                                                     void *argp);

/* Signals into the FSM that the HCI Read Local Amp Info has
 * failed. */
extern void CsrBtAmpmLocalHciReadAmpInfoFailure(CsrBtAmpmInstance *inst,
                                                CsrBtAmpmLocalAmp *pLocalAmp);

/* Signals into the FSM that the HCI Read Local Amp Assoc has
 * failed. */
extern void CsrBtAmpmLocalHciReadAmpAssocFailure(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLocalAmp *pLocalAmp);

/* Signals into the FSM that the HCI Write Remote Amp Assoc has
 * failed. */
extern void CsrBtAmpmLocalHciWriteRemoteAmpAssocFailure(CsrBtAmpmInstance *inst,
                                                        CsrBtAmpmLocalAmp *pLocalAmp);

/* Passes the return parameters from a HCI Command Complete event for
 * a HCI Read Local Amp Info. */
extern void CsrBtAmpmLocalHciReadLocalAmpInfo(CsrBtAmpmInstance *inst,
                                              CsrBtAmpmLocalAmp *pLocalAmp,
                                              CsrBtAmpHciReadLocalAmpInfoReturn * pReadLocalAmpInfoReturn);

/* Passes the return parameters from a HCI Command Complete event for
 * a HCI Read Local Amp Assoc. */
extern void CsrBtAmpmLocalHciReadLocalAmpAssoc(CsrBtAmpmInstance *inst,
                                               CsrBtAmpmLocalAmp *pLocalAmp,
                                               CsrBtAmpHciReadLocalAmpAssocReturn *pReadLocalAmpAssocReturn);

/* Passes the return parameters from a HCI Command Complete event for
 * a HCI Write Remote Amp Assoc. */
extern void CsrBtAmpmLocalHciWriteRemoteAmpAssoc(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLocalAmp *pLocalAmp,
                                                 CsrBtAmpHciWriteRemoteAmpAssocReturn *pWriteRemoteAmpAssocReturn);


/* Requests the AMP to transmit the supplied HCI Primitive.  If the
 * AMP is currently waiting for the status event for a previous HCI
 * command then the primitive will be queued for transmission. */
extern void CsrBtAmpmLocalHciTxCmd(CsrBtAmpmInstance *inst,
                                   CsrBtAmpmId id,
                                   void *pRequester,
                                   CsrUint8 *pHciPrim);

/* Requests the AMP to remove any HCI Commands from the TX queue for
 * the specified requester. */
extern void CsrBtAmpmLocalRemoveHciTxCmds(CsrBtAmpmInstance *inst,
                                          CsrBtAmpmId Id,
                                          void *pRequester);

/* Handles the status response to the transmission of a HCI COMMAND
 * the function chaecks that the first entry in the queue matches the
 * status and will transmit the next queued command if there is
 * one. */
extern void *CsrBtAmpmLocalStatusForQueuedHciCmd(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLocalAmp *pLocalAmp,
                                                 CsrUint16 OpCode);

/* Will check that the current head of queued HCI Cmds matches the
 * specified op code, and will remove the head element. If any further
 * HCI CMds are queued then the next entry will be sent. */
extern void *CsrBtAmpmLocalRemoveQueuedHciCmd(CsrBtAmpmInstance *inst,
                                              CsrBtAmpmLocalAmp *pLocalAmp,
                                              CsrUint16 OpCode);

/* Register a local PAL with the AMPM */
extern void CsrBtAmpmLocalRegisterReq(CsrBtAmpmInstance *inst,
                                      const CsrBtAmpmPalRegisterReq *pAmpRegisterReq);

/* Unregister a local PAL with the AMPM */
extern void CsrBtAmpmLocalDeregisterReq(CsrBtAmpmInstance *inst,
                                        const CsrBtAmpmPalDeregisterReq *pAmpDeregisterReq);

CsrBool CsrBtAmpmLocalPowerOnRes(CsrBtAmpmInstance *inst,
                             CsrBtAmpmPowerOnRes *pAmpPowerOnRes);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_LOCAL_H */
