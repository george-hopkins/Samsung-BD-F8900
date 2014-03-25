#ifndef CSR_BT_AMPM_SDU_H
#define CSR_BT_AMPM_SDU_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_AMPM_DECODE_U8_IE(source, offset) \
    ((CsrUint8)((source)[(offset)]))

#define CSR_BT_AMPM_DECODE_U16_IE(source, offset) \
    ((CsrUint16)((((source)[((offset)+1)]) << 8) | \
                 ((source)[(offset)])))

#define CSR_BT_AMPM_DECODE_U32_IE(source, offset) \
    ((CsrUint32)((((source)[((offset)+3)]) << 24) | \
                (((source)[((offset)+2)]) << 16) | \
                (((source)[((offset)+1)]) <<  8) | \
                ((source)[(offset)])))

/* Common offsets for decoding SDUs */
#define CSR_BT_AMPM_SDU_CODE_OFFSET                                   0
#define CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET                             1
#define CSR_BT_AMPM_SDU_LENGTH_OFFSET                                 2
#define CSR_BT_AMPM_SDU_MESSAGE_OFFSET                                4

#define CSR_BT_AMPM_LIST_OFFSET_IN_CHANGE_IND                         4
#define CSR_BT_AMPM_LIST_OFFSET_IN_DISCOVER_RSP                       8

/* AMP Manager SDU Codes*/
#define CSR_BT_AMPM_COMMAND_REJECT                                    0x01
#define CSR_BT_AMPM_DISCOVER_REQUEST                                  0x02
#define CSR_BT_AMPM_DISCOVER_RESPONSE                                 0x03
#define CSR_BT_AMPM_CHANGE_NOTIFY                                     0x04
#define CSR_BT_AMPM_CHANGE_RESPONSE                                   0x05
#define CSR_BT_AMPM_GET_INFO_REQUEST                                  0x06
#define CSR_BT_AMPM_GET_INFO_RESPONSE                                 0x07
#define CSR_BT_AMPM_GET_AMP_ASSOC_REQUEST                             0x08
#define CSR_BT_AMPM_GET_AMP_ASSOC_RESPONSE                            0x09
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_REQUEST                      0x0A
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RESPONSE                     0x0B
#define CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_REQUEST                  0x0C
#define CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_RESPONSE                 0x0D

#define CSR_BT_AMPM_FIRST_SDU                                         CSR_BT_AMPM_COMMAND_REJECT
#define CSR_BT_AMPM_LAST_SDU                                          CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_RESPONSE
#define CSR_BT_AMPM_INVALID_SDU                                       0xFF

/* Minimum SDU sizes */
#define CSR_BT_AMPM_MIN_COMMAND_REJECT_SDU_SIZE                       6
#define CSR_BT_AMPM_MIN_DISCOVER_REQUEST_SDU_SIZE                     8
#define CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_SDU_SIZE                    8
#define CSR_BT_AMPM_MIN_CHANGE_NOTIFY_SDU_SIZE                        4
#define CSR_BT_AMPM_MIN_CHANGE_RESPONSE_SDU_SIZE                      4
#define CSR_BT_AMPM_MIN_GET_INFO_REQUEST_SDU_SIZE                     5
#define CSR_BT_AMPM_MIN_GET_INFO_RESPONSE_SDU_SIZE                    22
#define CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_SDU_SIZE                5
#define CSR_BT_AMPM_MIN_GET_AMP_ASSOC_RESPONSE_SDU_SIZE               6
#define CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_SDU_SIZE         6
#define CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_RESPONSE_SDU_SIZE        7
#define CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_REQUEST_SDU_SIZE     6
#define CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_RESPONSE_SDU_SIZE    7

/* Minimum SDU length field (excludes the code, identifier and length fields */
#define CSR_BT_AMPM_MIN_COMMAND_REJECT_LENGTH_VALUE                    2
#define CSR_BT_AMPM_MIN_DISCOVER_REQUEST_LENGTH_VALUE                  4
#define CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_LENGTH_VALUE                 4
#define CSR_BT_AMPM_MIN_CHANGE_NOTIFY_LENGTH_VALUE                     0
#define CSR_BT_AMPM_MIN_CHANGE_RESPONSE_LENGTH_VALUE                   0
#define CSR_BT_AMPM_MIN_GET_INFO_REQUEST_LENGTH_VALUE                  1
#define CSR_BT_AMPM_MIN_GET_INFO_RESPONSE_LENGTH_VALUE                 18
#define CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_LENGTH_VALUE             1
#define CSR_BT_AMPM_MIN_GET_AMP_ASSOC_RESPONSE_LENGTH_VALUE            2
#define CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_LENGTH_VALUE      2
#define CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_RESPONSE_LENGTH_VALUE     3
#define CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_REQUEST_LENGTH_VALUE  2
#define CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_RESPONSE_LENGTH_VALUE 3

#define CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY                                   3
#define CSR_BT_AMPM_SIZEOF_VALID_AMP_INFO_FIELDS                            16

/* AMP Get Info IE offsets for decoding */
#define CSR_BT_AMPM_GET_INFO_RSP_AMP_ID_OFFSET                           (CSR_BT_AMPM_SDU_MESSAGE_OFFSET)
#define CSR_BT_AMPM_GET_INFO_RSP_STATUS_OFFSET                           (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 1)
#define CSR_BT_AMPM_GET_INFO_RSP_TOTAL_BW_OFFSET                         (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 2)
#define CSR_BT_AMPM_GET_INFO_RSP_GUARANTEED_BW_OFFSET                    (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 6)
#define CSR_BT_AMPM_GET_INFO_RSP_MIN_LATENCY_OFFSET                      (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 10)
#define CSR_BT_AMPM_GET_INFO_RSP_PAL_CAPABILITIES_OFFSET                 (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 14)
#define CSR_BT_AMPM_GET_INFO_RSP_AMP_ASSOC_LEN_OFFSET                    (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 16)

/* AMP Manager verion number */
#define CSR_BT_AMPM_MANAGER_VERSION                                      0x00

#define CSR_BT_AMPM_MANAGER_FEATURES                                     0x0000

#define CSR_BT_AMPM_SIZEOF_MTU_MPS_SIZE                                  2
#define CSR_BT_AMPM_DISCOVER_EXT_FEATURE_MASK_OFFSET                     (CSR_BT_AMPM_SDU_MESSAGE_OFFSET + CSR_BT_AMPM_SIZEOF_MTU_MPS_SIZE)
#define CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS                               0x8000
#define CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK                              2

/* Returns the next free Identifier to be used on the interface */
extern CsrUint8 CsrBtAmpmSduAllocIdentifier(CsrBtAmpmInstance * inst);

/* Allocates a message blk and populates with an AMP COMMAND REJECT
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpCommandReject(CsrUint8 identifier);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP COMMAND REJECT SDU */
extern CsrBool CsrBtAmpmSduValidateAmpCommandReject(
    const CsrUint16 length,
    const CsrUint8 * pSdu);


/* Allocates a message blk and populates with an AMP DISCOVER REQUEST
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpDiscoverRequest(
    CsrUint8 identifier,
    CsrUint16 supportedMTUSize);

/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP DISCOVER REQUEST SDU */
extern CsrBool CsrBtAmpmSduValidateAmpDiscoverRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP DISCOVER RESPONSE
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpDiscoverResponse(
    CsrBtAmpmInstance * inst,
    CsrUint8 identifier,
    CsrUint16 supportedMTUSize);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP DISCOVER RESPONSE SDU */
extern CsrBool CsrBtAmpmSduValidateAmpDiscoverResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP CHANGE NOTIFY
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpChangeNotify(
    CsrBtAmpmInstance * inst,
    CsrUint8 identifier);

/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP CHANGE NOTIFY SDU */
extern CsrBool CsrBtAmpmSduValidateAmpChangeNotify(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP CHANGE RESPONSE
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpChangeResponse(
    CsrUint8 identifier);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP CHANGE RESPONSE SDU */
extern CsrBool CsrBtAmpmSduValidateAmpChangeResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP GET INFO REQUEST
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpGetInfoRequest(
    CsrUint8 identifier,
    CsrBtAmpmId ampId);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP GET INFO REQUEST SDU */
extern CsrBool CsrBtAmpmSduValidateAmpGetInfoRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP GET INFO RESPONSE
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpGetInfoResponse(
    CsrUint8 identifier,
    CsrUint8 status,
    const CsrBtAmpmDataInfo * pAMPInfo);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP GET INFO RESPONSE SDU */
extern CsrBool CsrBtAmpmSduValidateAmpGetInfoResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP GET ASSOC REQUEST
 * SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpGetAmpAssocRequest(
    CsrUint8 identifier,
    CsrBtAmpmId ampId);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP GET ASSOC REQUEST SDU */
extern CsrBool CsrBtAmpmSduValidateAmpGetAmpAssocRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP GET ASSOC
 * RESPONSE SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpGetAmpAssocResponse(
    CsrUint8 identifier,
    CsrUint8 status,
    CsrBtAmpmId ampId,
    CsrBtAmpmAmpAssoc * pAssoc);

/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP GET ASSOC RESPONSE SDU */
extern CsrBool CsrBtAmpmSduValidateAmpGetAmpAssocResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP CREATE PHYSICAL
 * LINK REQUEST SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpCreatePhysicalLinkRequest(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    const CsrBtAmpmAmpAssoc * pAMPAssoc);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP CREATE PHYSICAL LINK REQUEST SDU */
extern CsrBool CsrBtAmpmSduValidateAmpCreatePhysicalLinkRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP CREATE PHYSICAL
 * LINK RESPONSE SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    CsrUint8 status);

/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP CREATE PHYSICAL LINK RESPONSE SDU */
extern CsrBool CsrBtAmpmSduValidateAmpCreatePhysicalLinkResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates with an AMP DISCONNECT
 * PHYSICAL LINK REQUEST SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkRequest(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId);

/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP DISCONNECT PHYSICAL LINK REQUEST SDU */
extern CsrBool CsrBtAmpmSduValidateAmpDisconnectPhysicalLinkRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu);

/* Allocates a message blk and populates it with an AMP DISCONNECT
 * PHYSICAL LINK RESPONSE SDU */
extern CsrMblk *CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkResponse(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    CsrUint8 status);


/* Performs simple validation of the supplied data to ensure it is a
 * correctly formed AMP DISCONNECT PHYSICAL LINK RESPONSE SDU */
extern CsrBool CsrBtAmpmSduValidateAmpDisconnectPhysicalLinkResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu);


/* Decodes the contents of an AMP GET INFO RESPONSE SDU */
extern void CsrBtAmpmSduDecodeAmpGetInfoResponse(
    const CsrUint8 * pAmpGetInfoRsp,
    CsrUint8 * pSduIdentifier,
    CsrUint8 * pStatus,
    CsrBtAmpmDataInfo * pAMPInfo);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_SDU_H */
