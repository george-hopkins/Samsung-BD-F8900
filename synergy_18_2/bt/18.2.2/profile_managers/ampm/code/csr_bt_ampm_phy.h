#ifndef CSR_BT_AMPM_PHY_H
#define CSR_BT_AMPM_PHY_H

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Struct containing basic search params required when searching for PhyLink Instance */
typedef struct
{
    BD_ADDR_T bd_addr;
    CsrBtAmpmId amp_id;
    CsrBtConnId cid;
    CsrUint8 sdu_identifier;
} CsrBtAmpmPhySearchParams;

/* Struct of a PhyLink Instance */
typedef struct
{
    CsrBtAmpmGenListMember header;
    fsm_state_t state;
    CsrBtAmpmBtdev *pBtDevice;
    BD_ADDR_T bdAddr;
    CsrUint8 ampPLinkHandle;
    CsrBool haveReadLsto;
    CsrUint32 linkSupervisionTimeout;
    CsrUint8 sduIdentifier;
    CsrUint8 expectedResponseSdu;
    CsrUint8 ampCreatePhysicalRspSduIdentifier;
    CsrSchedTid sduResponseTid;
    CsrSchedTid hciResponseTid;
    CsrBtAmpmLocalAmp *pLocalAMPInfo;
    CsrBtAmpmDataInfo *pRemoteAMPInfo;
    CsrBool initiator;
    CsrBool startedLinkCreation;
    CsrBool connected;
    CsrBool verifyResponseRequired;
    CsrUint16 verifyIdentifier;
    CsrBool hciDisconnectNeeded;
    CsrBool peerDisconnectNeeded;
    CsrBool sendPeerDisconnectRspNeeded;
    CsrBool dontDisconnectDm;
} CsrBtAmpmPhyLink;

/* Should this physical link not be started as failure testing is
 * enabled */
extern CsrBool CsrBtAmpmPhyDontStartPhysLink(CsrBtAmpmInstance *inst,
                                             CsrBtAmpmPhyLink *phylink);

/* Is collision triggering enabled */
extern CsrBool CsrBtAmpmPhyCollisionRequired(CsrBtAmpmInstance *inst);

/* Toggle testing feature on/off */
extern void CsrBtAmpmPhySetDontStartPhyLink(CsrBtAmpmInstance *inst,
                                            CsrBtAmpmTestDontStartPhyLinkReq *prim);

/* Toggle testing feature on/off */
extern void CsrBtAmpmPhySetPhyCollision(CsrBtAmpmInstance *inst,
                                        CsrBtAmpmTestPhyCollisionReq *prim);


/* Returns the value of the handle associated with the physical
 * link. */
CsrUint8 CsrBtAmpmPhyGetHandle(const CsrBtAmpmPhyLink * pPhyLink);


/* Returns the value of the local amp id associated with the physical
 * link. */
CsrBtAmpmId CsrBtAmpmPhyGetLocalAmpId(const CsrBtAmpmPhyLink * pPhyLink);


/* Returns the value of the remote amp id associated with the physical
 * link. */
CsrBtAmpmId CsrBtAmpmPhyGetRemoteAmpId(const CsrBtAmpmPhyLink * pPhyLink);


/* Returns whether the specified physical link is connected or not. */
CsrBool CsrBtAmpmPhyIsConnected(const CsrBtAmpmPhyLink * pPhyLink);


/* Returns whether the specified physical link is disconnecting or
 * not. */
CsrBool CsrBtAmpmPhyIsDisconnecting(const CsrBtAmpmPhyLink * pPhyLink);


/* Attempts to find a Phy Link instance that matches the specified BT
 * Device address */
CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByBdaddr(CsrBtAmpmInstance * inst,
                                          BD_ADDR_T bd_addr);


/* Attempts to find a Phy Link instance that matches the specified BT
 * Device address and Local AMP Id */
extern CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByBdaddrAndLocalAmpId(CsrBtAmpmInstance * inst,
                                                              BD_ADDR_T bd_addr,
                                                              CsrBtAmpmId id);


/* Attempts to find a Phy Link instance that matches the specified
 * Local AMP Id */
extern CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByLocalAmpId(CsrBtAmpmInstance * inst,
                                                     CsrBtAmpmId id);


/* Attempts to find a Phy Link instance that matches the specified
 * Remote AMP Id */
extern CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByRemoteAmpId(CsrBtAmpmInstance * inst,
                                                      CsrBtAmpmId id);


/* Attempts to find a Phy Link instance that matches the specified
 * L2CAP Channel ID and Sdu Identifier */
extern CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByBdaddrAndSduIdentifier(CsrBtAmpmInstance * inst,
                                                                 CsrBtAmpmPhySearchParams * pSearchParams);


/* Attempts to find a Phy Link instance that matches the specified AMP
 * Physical Handle */
extern CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByAmpPhyHandle(CsrBtAmpmInstance * inst,
                                                       CsrSchedQid amp_phy_handle);

/* Checks if an active Phy Link instance that matches the specified
 * pointer */
extern CsrBool CsrBtAmpmPhyInstanceExists(CsrBtAmpmInstance * inst,
                                          CsrBtAmpmPhyLink * pPhyLink);


/* Initialises any global data associated with the Physical Link. */
extern void CsrBtAmpmPhyInit(CsrBtAmpmInstance * inst);

/* Deinitialises any global data associated with the Physical Link. */
extern void CsrBtAmpmPhyDeinit(CsrBtAmpmInstance * inst);

/* Attempts to handle the successful HCI Command Status for HCI
 * Create/Accept Physical Link Request */
extern void CsrBtAmpmPhyLinkCommandStatusSuccess(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmPhyLink * pPhyLink);

/* Attempts to handle the failed HCI Command Status for HCI
 * Create/Accept Physical Link Request */
extern void CsrBtAmpmPhyLinkCommandStatusNeg(CsrBtAmpmInstance * inst,
                                             CsrBtAmpmPhyLink * pPhyLink);


/* Updates the Link Supervision timeout value associated with to
 * physical link instance */
extern void CsrBtAmpmPhyHciReadSupervisionTimeout(CsrBtAmpmInstance * inst,
                                                  CsrBtAmpmPhyLink * pPhyLink,
                                                  CsrBtAmpHciReadLinkSupervisionTimeoutReturn * pReturn);


/* Updates the options associated with controlling the generation of
 * DISCONNECT PHYSICAL LINK REQUEST for testing purposes. This
 * behaviour will affect all physical links when enabled. */
extern void CsrBtAmpmPhySetCancelCreatePhyLinkOptions(CsrBtAmpmInstance * inst,
                                                      CsrBool enabled,
                                                      CsrBool afterCreatePhyLinkRsp,
                                                      CsrBool invalidResponderAmpId,
                                                      CsrUint32 delayAfterCreatePhyLinkReq);

/* Handles a response timeout and signalling the event into the
 * supplied Phy Link FSM instance (if it is still valid). This will
 * also trigger disconnection of the AMP Manager fixed channel. */
extern void CsrBtAmpmPhySduResponseTimeoutInd(CsrUint16 ignored,
                                              void *pFsmInstance);


/* Handles a response timeout and signalling the event into the
 * supplied Phy Link FSM instance (if it is still valid). */
extern void CsrBtAmpmPhyHciResponseTimeoutInd(CsrUint16 ignored,
                                              void *pFsmInstance);

/* Handles a cancel timeout and signalling the disconnect event into
 * the supplied Phy Link FSM instance (if it is still valid). This
 * should trigger the cancelling of the physical link
 * establishment. */
extern void CsrBtAmpmPhySendCancelReqTimeoutInd(CsrUint16 ignored,
                                                void *pPhyLinkInstance);


/* Finds the first physical link instance that matches the BT address
 * specified and indicates that a successful AMP discovery has been
 * performed.  The assumption is that there is only ever a single
 * Physical link pending on a discovery process. The AMP information
 * received from the other device is checked to see if an appropriate
 * AMP is available to proceed with the physical link. Otherwise the
 * physical link creation is aborted. */
extern void CsrBtAmpmPhyAmpDiscoverySuccess(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmBtdev * pBtDevice);


/* Finds the first physical link instance that matches the BT address
 * specified and indicates that a AMP discovery has failed.  The
 * assumption is that there is only ever a single Physical link
 * pending on a discovery process. The only option is to abort the amp
 * physical link connection as we know nothing about the AMPs at the
 * other device. */
extern void CsrBtAmpmPhyAmpDiscoveryFailure(
    CsrBtAmpmInstance * inst,
    BD_ADDR_T bd_addr);


/* Finds the first physical link instance that matches the BT address
 * specified and triggers the disconnection of the Physical link
 * locally only. */
extern void CsrBtAmpmPhyLocalDisconnectOnly(
    CsrBtAmpmInstance * inst,
    BD_ADDR_T bd_addr);


/* Finds the all the physical link instances that are using the
 * specified Amp ID and triggers the disconnection of the Physical
 * link without signalling to the AMP. */
extern void CsrBtAmpmPhyLocalAmpRemoved(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmId ampId);


/* Finds the first physical link instance that matches the supplied
 * local amp id and indicates the result of attempting to write the
 * supplied remote amp assoc to the amp. */
extern void CsrBtAmpmPhyLocalAmpWriteRemoteAssocResponse(
    CsrBtAmpmInstance * inst,
    CsrUint8 PhysicalLinkHandle,
    CsrBool AssocWritten);


/* Checks to see if the supplied instance pointer is a physical link
 * instance and if so passes the result of reading the amp assoc and
 * the amp assoc (if successful) into the physical link setup
 * process. */
extern void CsrBtAmpmPhyLocalAmpAssoc(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPossiblePhyLink,
    CsrBtAmpmId ampId,
    CsrBtAmpmAmpAssoc * pAMPAssoc);

/* Handles an incoming AMP COMMAND REJECT sdu */
extern CsrBool CsrBtAmpmPhyAmpCommandReject(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid);


/* Handles an incoming AMP GET ASSOC RESPONSE sdu */
extern void CsrBtAmpmPhyAmpGetAmpAssocResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid);


/* Handles an incoming AMP CREATE PHYSICAL LINK REQUEST sdu */
extern void CsrBtAmpmPhyAmpCreatePhysicalLinkRequest(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid);


/* Handles an incoming AMP CREATE PHYSICAL LINK RESPONSE sdu */
extern void CsrBtAmpmPhyAmpCreatePhysicalLinkResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid);


/* Handles an incoming AMP DISCONNECT PHYSICAL LINK REQUEST sdu */
extern void CsrBtAmpmPhyAmpDisconnectPhysicalLinkRequest(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid);


/* Handles an incoming AMP DISCONNECT PHYSICAL LINK RESPONSE sdu */
extern void CsrBtAmpmPhyAmpDisconnectPhysicalLinkResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid);

/* Handles an incoming HCI Channel select. */
extern void CsrBtAmpmPhyHciChannelSelect(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPalHciEventReq *ampm_hci_prim);


/* Handles an incoming HCI Physical link complete */
extern void CsrBtAmpmPhyHciPhysicalLinkComplete(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPalHciEventReq * ampm_hci_prim);


/* Handles the final HCI primitive from a physical link disconnect.
 * This might be the final complete or a status. */
extern void CsrBtAmpmPhyHciDisconnectPhysicalLinkComplete(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink,
    CsrBtAmpHciStatusCode status,
    CsrUint8 reason);

/* Handles an AMP power on response caused by a locally initiated move */
void CsrBtAmpmPhyPowerOnRes(CsrBtAmpmInstance * inst, CsrBtAmpmPowerOnRes *res);

/* Either returns an existing physical link that is using the
 * specified AMPs between the local device and specified BT Address or
 * if there is no existing link will attempt to create a new one. */
extern CsrBtAmpmPhyLink *CsrBtAmpmPhyGetPhyForAmps(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    BD_ADDR_T bd_addr);


/* Triggers the disconnection of the specified physical link Without
 * performing any checks on its useage. */
extern void CsrBtAmpmPhyDisconnect(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink);

/* Checks if the physical link can be disconnected if there are no
 * more logical links or L2ca Channels using it. */
extern void CsrBtAmpmPhyTriggerDisconnectIfRequired(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_PHY_H */
