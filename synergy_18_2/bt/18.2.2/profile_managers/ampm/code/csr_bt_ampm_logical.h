#ifndef CSR_BT_AMPM_LOGICAL_H
#define CSR_BT_AMPM_LOGICAL_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef CsrUint16 CsrBtAmpmLogicalHandle;

/* Struct of a LogicalLink Instance */
typedef struct
{
    CsrBtAmpmGenListMember header;
    CsrBtAmpmPhyLink *pPhyLink;
    CstBtAmpmGenListAnchor *pLocalL2caChanListAnchor;
    CsrBtAmpmL2cap *pL2caChanTriggedConnection;
    CsrBtAmpmL2cap *pL2caChanTriggedModify;
    CsrBtAmpAmpFlowSpec txFlowspec;
    CsrBtAmpAmpFlowSpec rxFlowspec;
    CsrBtAmpAmpFlowSpec previousTxFlowspec;
    CsrBtAmpAmpFlowSpec previousRxFlowspec;
    CsrBtAmpmLogicalHandle handle;
    BD_ADDR_T bd_addr;
    CsrSchedTid responseTid;
    CsrBool alreadyAttemptedCancel;
    CsrBool waitingToStart;
    fsm_state_t state;
} CsrBtAmpmLogicalLink;

/* Toggle testing feature on/off */
extern void CsrBtAmpmLogSetDontStartLogicalLink(CsrBtAmpmInstance *inst,
                                                CsrBtAmpmTestDontStartLogLinkReq *prim);

/* Should a logical link not be started as failure testing is
 * enabled */
extern CsrBool CsrBtAmpmLogDontStartLogicalLink(CsrBtAmpmInstance *inst);

/* Description: function creates a new Logical Link instance */
extern CsrBtAmpmLogicalLink *CsrBtAmpmLogicalNew(CsrBtAmpmInstance *inst,
                                                 BD_ADDR_T bd_addr);

/* Associates the supplied Logical Link instance with the L2CAP
 * channel. */
extern void CsrBtAmpmLogicalAssociateL2capChan(CsrBtAmpmLogicalLink *pLogLink,
                                               CsrBtAmpmL2cap *pL2caChan);

/* Deassociates the supplied Logical Link instance from the L2CAP
 * channel. */
extern void CsrBtAmpmLogicalDeasssociateL2capChan(CsrBtAmpmLogicalLink *pLogLink,
                                                  CsrBtAmpmL2cap *pL2caChan);


/* Associates the supplied Logical Link instance with the physical
 * link instance. */
extern void CsrBtAmpmLogicalAssociatePhyLink(CsrBtAmpmLogicalLink *pLogLink,
                                             CsrBtAmpmPhyLink *pPhyLink);


/* Deassociates the supplied Logical Link instance with the physical
 * link instance. */
extern void CsrBtAmpmLogicalDeassociatePhyLink(CsrBtAmpmLogicalLink *pLogLink,
                                               CsrBtAmpmPhyLink *pPhyLink);


/* Returns the number of logical links using the specified physical
 * link. */
extern CsrUint8 CsrBtAmpmLogicalLinksUsingPhyLink(CsrBtAmpmInstance *inst,
                                                  CsrBtAmpmPhyLink *pPhyLink);

/* Finds a best effort logical link to the specified physical link.
 * As best effort L2ca Channels are aggregated to run over a single
 * logical link the assumption is that there is only ever a single
 * best effort logical link to a single physical link. */
extern CsrBtAmpmLogicalLink *CsrBtAmpmLogicalExistingBestEffortLink(CsrBtAmpmInstance *inst,
                                                                    CsrBtAmpmPhyLink *pPhyLink);

/* Finds a logical link and corresponding l2ca channel instance that
 * matches the specified Cid */
extern CsrBool CsrBtAmpmLogicalL2capChanGetByLocalCid(CsrBtAmpmInstance *inst,
                                                      CsrBtAmpmLogicalLink **ppLogLink,
                                                      CsrBtAmpmL2cap ** ppL2caChan,
                                                      CsrBtConnId cid);

/* Finds a logical link instance that matches the specified handle */
extern CsrBtAmpmLogicalLink *CsrBtAmpmLogicalGetByHandle(CsrBtAmpmInstance *inst,
                                                         CsrBtAmpmLogicalHandle handle);

/* Performs any initialisation required by the logical link entity at
 * startup of the AMP Manager. */
extern void CsrBtAmpmLogicalInit(CsrBtAmpmInstance *inst);


/* Performs any deinitialisation required by the logical link entity
 * at shutdown of the AMP Manager. */
extern void CsrBtAmpmLogicalDeinit(CsrBtAmpmInstance *inst);

/* Returns TRUE if the specified logical link is connected. */
extern CsrBool CsrBtAmpmLogicalIsConnected(CsrBtAmpmLogicalLink *pLogLink);

/* Returns the bandwidth usage for the specified logical link and
 * indicates if the bandwidth is guaranteed or not. */
extern void CsrBtAmpmLogicalGetBandwidthUsage(CsrBtAmpmLogicalLink *pLogLink,
                                              CsrBool *pIsGuaranteed,
                                              CsrUint32 *pBandwidth);

/* Returns the bandwidth usage for the logical links using the
 * specified physical link */
void CsrBtAmpmLogicalGetBandwidthUsageForPhyLink(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmPhyLink *pPhyLink,
                                                 CsrUint32 *pUnguaranteedbandwidth,
                                                 CsrUint32 *pGuaranteedBandwidth);

/* Directs events to logical links associate with the specified
 * physical link. Note: As there is no direct association in the
 * direction of the physical link to the logical inks that might be
 * using it we need to come at it from theother direct and find the
 * logical links that are using the specified physical link. */
extern void CsrBtAmpmLogicalPhyLinkEvent(CsrBtAmpmInstance *inst,
                                         CsrBtAmpmPhyLink *pPhyLink,
                                         fsm_event_t event);

/* Generates the CREATE CHANNEL REQ event into the specified logical
 * link instance. */
extern void CsrBtAmpmLogicalCreateChannelReq(CsrBtAmpmInstance *inst,
                                             CsrBtAmpmLogicalLink *pLogLink,
                                             CsrBtAmpmL2cap *pL2caChan);

/* Generates the DISCONNECT CHANNEL REQ event into the specified
 * logical link instance. Note: If this is the only FlowSpec that the
 * logical link is supporting then the logical link will also be
 * disconnected. Otherwise the logical link will need to be
 * modified. */
extern void CsrBtAmpmLogicalDisconnectChannelReq(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLogicalLink *pLogLink,
                                                 CsrBtAmpmL2cap *pL2caChan);

/* Called in event of a timer expiring. It attempts to find the
 * specified logical link instance (which might have been deleted due
 * to other reasons) and signal the timeout event into the FSM.  A
 * timeout only occurs in error scenarios, so after hanlding the
 * timeout in the FSM it is assumed that the logical link is no longer
 * required and the instance is deleted. */
extern void CsrBtAmpmLogicalHciResponseTimeoutInd(CsrUint16 ignored,
                                                  const void *pFsmInstance);

/* Handles the AMP HCI LOGICAL LINK COMPLETE checking the status
 * returned to determine whether the link was established successfully
 * or not. Then appropriate actions within the Logical Link FSM are
 * triggered. */
extern void CsrBtAmpmLogicalHciLogicalLinkComplete(CsrBtAmpmInstance *inst,
                                                   CsrBtAmpmPalHciEventReq *pAmpmHciPrim);

/* Handles the AMP HCI FLOW SPEC MODIFY COMPLETE checking the status
 * returned to determine whether the link was modified successfully or
 * not. Then appropriate actions within the Logical Link FSM are
 * triggered. */
extern void CsrBtAmpmLogicalHciFlowSpecModifyComplete(CsrBtAmpmInstance *inst,
                                                      CsrBtAmpmPalHciEventReq *pAmpmHciPrim);

/* Handles the AMP HCI DISCONNECT LOGICAL LINK COMPLETE checking the
 * status returned to determine whether the link was disconnected
 * successfully or not. Then appropriate actions within the Logical
 * Link FSM are triggered. */
extern void CsrBtAmpmLogicalHciDisconnectLogicalLinkComplete(CsrBtAmpmInstance *inst,
                                                             CsrBtAmpmPalHciEventReq *pAmpmHciPrim);

/* Handles the AMP HCI COMMAND STATUS in the case of the command
 * failing */
extern void CsrBtAmpmLogicalHciCommandStatusFailure(CsrBtAmpmInstance *inst,
                                                    CsrBtAmpmLogicalLink *pLogLink);

/* Returns the result of the HCI Create Logical Link Cancel command */
extern void CsrBtAmpmLogicalHciLogicalLinkCancel(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLogicalLink *pLogLink,
                                                 CsrBtAmpHciLogicalLinkCancelReturn *pReturn);

/* Returns the unguaranteed bandwidth useage. */
extern CsrUint32 AMPM_LogLink_GetUnguaranteedBandwidthUsage(CsrBtAmpmLogicalLink *pLogLink);

/* Returns the guaranteed bandwidth useage. */
extern CsrUint32 AMPM_LogLink_GetGuaranteedBandwidthUsage(CsrBtAmpmLogicalLink *pLogLink);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_LOGICAL_H */
