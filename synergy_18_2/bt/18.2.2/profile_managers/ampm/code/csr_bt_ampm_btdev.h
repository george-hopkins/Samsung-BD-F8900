#ifndef CSR_BT_AMPM_BT_DEVICE_H
#define CSR_BT_AMPM_BT_DEVICE_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    AMPM_STATUS_UNKNOWN,
    AMPM_STATUS_NOT_PRESENT,
    AMPM_STATUS_PRESENT
} CsrBtAmpmBtdevAmpmStatus;

typedef struct
{
    CsrBtAmpmGenListMember header;
    fsm_state_t state;
    BD_ADDR_T bdAddr;
    CsrBtAmpmBtdevAmpmStatus ampManagerStatus;
    CstBtAmpmGenListAnchor *pSendAmpSduReqListAnchor;
    CsrBool hasBeenDiscovered;
    CsrBool haveAllRemoteAmpInfo;
    CsrUint8 numOfAMPs;
    CsrBtAmpmDataRemoteAmp *pAMP_ListEntry;
    CstBtAmpmGenListAnchor *pRemoteAmpInfoListAnchor;
    CsrUint8 sduIdentifier;
    CsrBtConnId cid;
    CsrSchedTid sduResponseTimer;
    CsrUint16 getInfoTypeRequested;
} CsrBtAmpmBtdev;

typedef struct
{
    CsrBtAmpmGenListMember header;
    CsrBool infoValid;
    CsrBtAmpmDataInfo info;
} CsrBtAmpmBtdevRemoteAmpInfo;

/* Attempts to find a btdev instance that matches the specified BT
 * Device address */
extern CsrBtAmpmBtdev *CsrBtAmpmBtdevGetByBdaddr(CsrBtAmpmInstance *inst,
                                                 BD_ADDR_T bd_addr);

/* Attempts to find a btdev instance that matches the specified
 * L2CAP Channel ID */
extern CsrBtAmpmBtdev *CsrBtAmpmBtdevGetByCid(CsrBtAmpmInstance *inst,
                                              CsrBtConnId cid);

/* Attempts to find the remote amp status associated with the
 * specified btdev instance for the AMP id specified */
extern CsrUint8 CsrBtAmpmBtdevRemoteAmpStatusGetById(CsrBtAmpmBtdev *pBtDevice,
                                                     CsrBtAmpmId Id);

/* Initialises any global data associated with the btdevs. */
extern void CsrBtAmpmBtdevInit(CsrBtAmpmInstance *inst);

/* Deinitialises any global data associated with the btdevs. */
extern void CsrBtAmpmBtdevDeinit(CsrBtAmpmInstance *inst);

/* Checks to see if the supplied instance pointer is a Bt Device
 * instance and if so generates an AMP GET AMP ASSOC RESPONSE SDU
 * using the supplied data. */
extern void CsrBtAmpmBtdevLocalAmpAssoc(CsrBtAmpmInstance *inst,
                                        CsrBtAmpmBtdev *pPossibleBtDevice,
                                        CsrUint8 SduIdentifier,
                                        CsrBtAmpmId ampId,
                                        CsrBtAmpmAmpAssoc *pAMPAssoc);

/* Iterate over any existing Bt Device instances and inform them that
 * there has been a change in the locally available AMPs */
extern void CsrBtAmpmBtdevLocalAmpChangeInd(CsrBtAmpmInstance *inst);

/* Attempts to find the remote amp info associated with the specified
 * btdev instance for the AMP id specified */
extern CsrBtAmpmBtdevRemoteAmpInfo *CsrBtAmpmBtdevRemoteAmpInfoGetById(CsrBtAmpmBtdev *pBtDevice,
                                                                       CsrBtAmpmId Id);

/* Find matching remote AMP controller ID given BD_ADDR and local AMP controller ID */
extern CsrBtAmpController CsrBtAmpmBtdevFindMatchingRemoteController(CsrBtAmpmInstance *inst,
                                                                    BD_ADDR_T *addr,
                                                                    CsrBtAmpController localId);

/* Checks the specified btdev instance for a remote amp that
 * matches the specified amp id and will return a pointer to the
 * entry. */
extern CsrBtAmpmDataRemoteAmp *CsrBtAmpmBtdevGetRemoteAmpInfoById(CsrBtAmpmBtdev *pBtDevice,
                                                                  CsrBtAmpmId ampId);

/* Checks the specified btdev instance for a remote amp that
 * matches the specified amp type and will return a pointer to the
 * entry. */
extern CsrBtAmpmDataRemoteAmp *CsrBtAmpmBtdevGetRemoteAmpInfoByType(CsrBtAmpmBtdev *pBtDevice,
                                                                    CsrBtAmpmType ampType);

/* If the Bt Device has already established that the AMP Manager is
 * present at the Bt Device then the SDU will be sent otherwise the
 * SDU is queued waiting for transmission. */
extern void CsrBtAmpmBtdevTestSduReq(CsrBtAmpmInstance *inst,
                                     CsrBtAmpmTestSduReq *pAmpmTestSduReq);

/* Trigger BT device discovery */
extern void CsrBtAmpmBtdevKickDiscovery(CsrBtAmpmInstance *inst,
                                        BD_ADDR_T addr);

/* Trigger the disconnection of the L2CAP AMP Manager fixed channel
 * for the specified btdev. */
extern void CsrBtAmpmBtdevDisconnectReq(CsrBtAmpmInstance *inst,
                                        CsrBtAmpmBtdev *pBtDevice,
                                        CsrBtAmpmResult result);


/* Indicates the disconnection of the L2CAP AMP Manager fixed channel
 * to the specified peer BT address. */
extern void CsrBtAmpmBtdevDisconnectInd(CsrBtAmpmInstance *inst,
                                        BD_ADDR_T bd_addr,
                                        CsrBtAmpmResult result);


/* Function for handling a AMP Manager SDU response timeout and
 * signalling the event into the supplied btdev FSM instance (if
 * it is still valid). This will trigger disconnection of the AMP
 * Manager fixed channel. */
extern void CsrBtAmpmBtdevSduResponseTimeoutInd(CsrUint16 ignored,
                                                const void *pFsmInstance);

/* Returns TRUE if the specified btdev instance has discovered the
 * remote AMPs present at the device */
extern CsrBool CsrBtAmpmBtdevIsDiscovered(CsrBtAmpmBtdev *pBtDevice);

/* Returns TRUE if the specified btdev instance has been discovered
 * and contains at least one real PAL */
extern CsrBool CsrBtAmpmBtdevIsAmpable(CsrBtAmpmBtdev *pBtDevice);

/* Returns the AMP manager status of the specified btdev
 * instance. */
extern CsrBtAmpmBtdevAmpmStatus CsrBtAmpmBtdevAmpManagerStatus(CsrBtAmpmBtdev *pBtDevice);

/* Handles an incoming L2CAP GETINFO CONFIRM As AMP Manager has to
 * check that the extended feature "fixed channels" is supported at
 * the peer btdev and then check that the AMP Manager fixed channel is
 * supported. */
extern void CsrBtAmpmL2capGetInfoCfm(CsrBtAmpmInstance *inst,
                                     L2CA_GETINFO_CFM_T *prim);

/* Handles an incoming L2CAP MAP FIXED CID CONFIRM Will find the
 * matching btdev instance based on the Bd addr and if the CId as
 * allocated successfully the event is signaled into the FSM,
 * otherwise the btdev instance is deleted. */
extern void CsrBtAmpmL2capMapFixedCidCfm(CsrBtAmpmInstance *inst,
                                         L2CA_MAP_FIXED_CID_CFM_T *prim);


/* Handles an incoming L2CAP MAP FIXED CID INDICATION Will find the
 * matching btdev instance based on the Bd addr or if one doesn't
 * exist create one (assuming that this has beaten the DM ACL OPEN
 * IND). */
extern void CsrBtAmpmL2capMapFixedCidInd(CsrBtAmpmInstance *inst,
                                         L2CA_MAP_FIXED_CID_IND_T *prim);

/* Handles the incoming indication from DM that it has opened an ACL
 * connection to a peer BT device. This indication will trigger the
 * AMP Manager to perform AMP Discovery towards the peer device. */
extern void CsrBtAmpmBtdevDmAclConnHandleInd(CsrBtAmpmInstance *inst,
                                             const DM_ACL_CONN_HANDLE_IND_T *prim);


/* Handles the incoming indication from DM that it has closed an ACL
 * connection to a peer BT device. This indication will trigger the
 * AMP Manager to perform local disconnection of any physical link
 * connected to the peer device. */
extern void CsrBtAmpmBtdevDmAclClosedInd(CsrBtAmpmInstance *inst,
                                         const DM_ACL_CLOSED_IND_T *prim);

/* The DM tells us whether the peer supports security mode 4 (SSP),
 * which is a prerequisite for AMP. If SSP is supported, progress the
 * FSM - otherwise cease peer AMP discovery immediately. */
extern void CsrBtAmpmBtdevRemoteFeaturesInd(CsrBtAmpmInstance *inst,
                                            DM_AMPM_REMOTE_FEATURES_IND_T *prim);

/* Handles an incoming AMP COMMAND REJECT sdu The only info contained
 * in the SDU it self is the identifier of the rejected SDU. We know
 * what channel it was received on so the first point to start is with
 * the BT device instance.  If this is not responsible for transiting
 * the associated SDU then we can start looking at AMPs and physical
 * links. */
extern CsrBool CsrBtAmpmBtdevAmpCommandReject(CsrBtAmpmInstance *inst,
                                              const CsrUint8 *pMsg,
                                              CsrBtConnId cid);


/* Handles an incoming AMP DISCOVER REQUEST sdu We should already have
 * created the required btdev FSM instance due to the L2CAP
 * channel creation. How we match or uniquely identify the instance
 * once we move to fixed channels needs to be resolved.  For the
 * moment we use the CID of the L2CAP channel to match the required BT
 * Device instance. */
extern void CsrBtAmpmBtdevAmpDiscoverRequest(CsrBtAmpmInstance *inst,
                                             const CsrUint8 *pMsg,
                                             CsrBtConnId cid);


/* Handles an incoming AMP DISCOVER RESPONSE sdu We should already
 * have created the required btdev FSM instance due to the L2CAP
 * channel creation. How we match or uniquely identify the instance
 * once we move to fixed channels needs to be resolved.  For the
 * moment we use the CID of the L2CAP channel to match the required BT
 * Device instance. */
extern void CsrBtAmpmBtdevAmpDiscoverResponse(CsrBtAmpmInstance *inst,
                                              const CsrUint8 *pMsg,
                                              CsrBtConnId cid);

/* Handles an incoming AMP CHANGE NOTIFY sdu For the moment we use the
 * CID of the L2CAP channel to match the required btdev
 * instance. */
extern void CsrBtAmpmBtdevAmpChangeNotify(CsrBtAmpmInstance *inst,
                                          const CsrUint8 *pMsg,
                                          CsrBtConnId cid);


/* Handles an incoming AMP CHANGE RESPONSE sdu For the moment we use
 * the CID of the L2CAP channel to match the required btdev
 * instance. */
extern void CsrBtAmpmBtdevAmpChangeResponse(CsrBtAmpmInstance *inst,
                                            const CsrUint8 *pMsg,
                                            CsrBtConnId cid);


/* Handles an incoming AMP GET INFO REQUEST sdu For the moment we use
 * the CID of the L2CAP channel to match the required btdev
 * instance. */
extern void CsrBtAmpmBtdevAmpGetInfoRequest(CsrBtAmpmInstance *inst,
                                            const CsrUint8 *pMsg,
                                            CsrBtConnId cid);


/* Handles an incoming AMP GET INFO RESPONSE sdu */
extern void CsrBtAmpmBtdevAmpGetInfoResponse(CsrBtAmpmInstance *inst,
                                             const CsrUint8 *pMsg,
                                             CsrBtConnId cid);

/* Handles an incoming AMP GET INFO REQUEST sdu For the moment we use
 * the CID of the L2CAP channel to match the required btdev instance.
 * The response SDU handling is in the physical link where the request
 * originates from. */
extern void CsrBtAmpmBtdevAmpGetAmpAssocRequest(CsrBtAmpmInstance *inst,
                                                const CsrUint8 *pMsg,
                                                CsrBtConnId cid);

#ifdef __cplusplus
}
#endif

#endif /*CSR_BT_AMPM_BT_DEVICE_H */
