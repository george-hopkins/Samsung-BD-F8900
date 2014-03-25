#ifndef CSR_BT_AMPM_L2CAP_H
#define CSR_BT_AMPM_L2CAP_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrBtAmpmGenListMember header;
    fsm_state_t state;
    BD_ADDR_T bd_addr;
    CsrBtConnId cid;
    L2CA_FLOWSPEC_T txFlowspec;
    L2CA_FLOWSPEC_T rxFlowspec;
    void *pUsingPhyLink;
    void *pUsingLogLink;
    CsrBtAmpController discReqLocalAmpId;
} CsrBtAmpmL2cap;

/* Initialises any global data associated with the L2CA Channels. */
extern void CsrBtAmpmL2capInit(CsrBtAmpmInstance * inst);

/* Deinitialises any global data associated with the L2CA Channels. */
extern void CsrBtAmpmL2capDeinit(CsrBtAmpmInstance * inst);

/* Main L2CAP primitive handler */
extern void CsrBtAmpmL2capHandler(CsrBtAmpmInstance * inst);

/* Description: Deletes the specified L2ca Channel instance. */
extern void CsrBtAmpmL2capDelete(CsrBtAmpmInstance * inst,
                                 CsrBtAmpmL2cap * pL2caChan);

/* Finds a the first l2ca channel that matches the specified channel
 * id in the supplied list. */
extern CsrBtAmpmL2cap *CsrBtAmpmL2capGetByCid(CsrBtAmpmInstance * inst,
                                              CstBtAmpmGenListAnchor * pListAnchor,
                                              CsrBtConnId cid);

/* Returns TRUE if the specified L2ca Channel instance is waiting for
 * a suitable physical link to be established. */
extern CsrBool CsrBtAmpmL2capWaitingForPhyLink(CsrBtAmpmL2cap * pL2caChan);

/* Returns TRUE if the specified L2ca Channel instance is established
 * over a logical link. */
extern CsrBool CsrBtAmpmL2capConnectedOverLogLink(CsrBtAmpmL2cap * pL2caChan);

/* Returns TRUE if the specified L2ca Channel instance is establishing
 * over a logical link. */
extern CsrBool CsrBtAmpmL2capConnectingOverLogLink(CsrBtAmpmL2cap * pL2caChan);

/* Will signal the event into the FSM if it is associated with the
 * specified physical link and not using a logical link (otherwise the
 * event comes via the logical link). */
extern void CsrBtAmpmL2capPhyEvent(CsrBtAmpmInstance * inst,
                                   CsrBtAmpmPhyLink * pPhyLink,
                                   fsm_event_t Event);

/* Returns the number of L2 Channel instances associated with the
 * specified physical link */
extern CsrUint8 CsrBtAmpmL2capL2caChansUsingPhyLink(CsrBtAmpmInstance * inst,
                                                    CsrBtAmpmPhyLink * pPhyLink);

/* Returns the TRUE if all L2 Channel instances associated with the
 *  specified physical link are disconnecting */
extern CsrBool CsrBtAmpmL2capUsingPhyLinkAreAllDisconnecting(CsrBtAmpmInstance * inst,
                                                             CsrBtAmpmPhyLink * pPhyLink);

/* Will find any L2ca channel instances that are using the specified
 * "old" physical link and switch their associatation to the "new"
 * physical link. */
extern void CsrBtAmpmL2capSwitchPhyLink(CsrBtAmpmInstance * inst,
                                        CsrBtAmpmPhyLink * pOldPhyLink,
                                        CsrBtAmpmPhyLink * pNewPhyLink);

/* Handles the DM AMPM CONNECT IND received from DM when it wants to
 * attempt to route a L2CAP ACL channel over an AMP.  If a suitable
 * AMP is available and there is no physical link already established
 * then establishment of one is triggered.  If the physical link is
 * established then the logical link establishment proceeds.
 * Check if best effort requested:
 *    Yes - check if a best effort channel already exists
 *           No - create a new one
 *           Yes - then modify link QoS (i.e. merge best effort channels).
 *    No - create now logical link for flow spec.
 * Keeping simple at the moment and assume just a single best effort
 * logical link.
 */
extern void CsrBtAmpmL2capDmAmpmConnectInd(CsrBtAmpmInstance * inst,
                                           DM_AMPM_CONNECT_IND_T * prim);

/* Handles the DM AMPM CONNECT CHANNEL IND received from DM when DM
 * has obtained both the Tx & Rx flow specs for the L2CAP channel and
 * has reached tha point that the logical link can be established
 * properly. */
extern void CsrBtAmpmL2capDmAmpmConnectChannelInd(CsrBtAmpmInstance * inst,
                                                  DM_AMPM_CONNECT_CHANNEL_IND_T * prim);

/* Handles the DM AMPM DISCONNECT CHANNEL IND received from DM */
extern void CsrBtAmpmL2capDmAmpmDisconnectChannelInd(CsrBtAmpmInstance * inst,
                                                     DM_AMPM_DISCONNECT_CHANNEL_IND_T * pDmAmpmPrim);

/* Generates the PHY LINK READY IND event into the specified L2CAP
 * channel instance. */
extern void CsrBtAmpmL2capPhyReadyInd(CsrBtAmpmInstance * inst,
                                      CsrBtAmpmL2cap * pL2caChan);

/* Generates the CHANNEL READY IND event into the specified L2CAP
 * channel instance. */
extern void CsrBtAmpmL2capChannelReadyInd(CsrBtAmpmInstance * inst,
                                          CsrBtAmpmL2cap * pL2caChan);

/* Generates the CHANNEL DISCONNECT IND event into the specified L2CAP
 * channel instance. */
extern void CsrBtAmpmL2capChannelDisconnectInd(CsrBtAmpmInstance * inst,
                                               CsrBtAmpmL2cap * pL2caChan);

/* Generates the LINK FAILURE IND event into the specified L2CAP
 * channel instance. */
extern void CsrBtAmpmL2capLinkFailureInd(CsrBtAmpmInstance * inst,
                                         CsrBtAmpmL2cap * pL2caChan);

/* Generates the LINK MODIFIED SUCCESS IND event into the specified
 * L2CAP channel instance. This represents either successful addition
 * of a new best effort L2Ca channel instance to an existing logical
 * link or removal of a L2Ca channel from a logical link that still
 * has other L2Ca channels associated with it. */
extern void CsrBtAmpmL2capLinkModifiedSuccessInd(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmL2cap * pL2caChan);

/* Generates the LINK MODIFIED FAILURE IND event into the specified
 * L2CAP channel instance. This represents either failure to add a new
 * best effort L2Ca channel instance to an existing logical link or
 * removal of a L2Ca channel from a logical link that still has other
 * L2Ca channels associated with it. */
extern void CsrBtAmpmL2capLinkModifiedFailureInd(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmL2cap * pL2caChan);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_L2CAP_H */
