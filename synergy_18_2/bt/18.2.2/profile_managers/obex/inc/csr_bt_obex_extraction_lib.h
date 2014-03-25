#ifndef CSR_BT_OBEX_EXTRACTION_LIB_H__
#define CSR_BT_OBEX_EXTRACTION_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

/* Needed for CsrBtObexSession */
#include "csr_bt_obex_lookup_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

extern CsrUint16  CsrBtObexGetMaxPacketLength(CsrUint8 *obexPacket);
extern CsrUint8   *CsrBtObexCreateName(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *nameLength);
extern CsrBool    CsrBtObexGetNameOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset);
extern CsrBool    CsrBtObexGetDestNameOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset);
#ifdef CSR_BT_INSTALL_OBEX_UTIL_BODY_OFFSET 
extern CsrBool    CsrBtObexGetBodyOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *bodyType,
                                   CsrUint16 *bodyOffset, CsrUint16 *bodyLength);
#endif
extern CsrUint8   CsrBtObexGetBodyHeaderOffset(CsrBtObexSession session, CsrUint8 *pObexData,
                                         CsrUint16 *headerOffset, CsrUint16 *headerLength, CsrUint8 *isFinal);
#ifdef CSR_BT_INSTALL_OBEX_UTIL_COPY_IMAGE
extern CsrBool    CsrBtObexCopyDecodedImgHandle(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *imgHandle);
#endif
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
extern CsrBool    CsrBtObexGetConnectionId(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint32 *connectionId);
#endif
extern CsrBool    CsrBtObexCopyAuthResponse(CsrBtObexSession session, CsrUint8 *obexPacket,
                                            CsrUint8 *requestDigest, CsrUint8 *userId, CsrUint8 *userIdLength);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
extern CsrBool    CsrBtObexCreateAuthChallenge(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *nonce, CsrUint8 *options,
                                         CsrBool *optionsPresent, CsrUint8 **realm, CsrUint16 *realmLength);
#endif
extern CsrUint8   CsrBtObexGetTypeHeader(CsrBtObexSession session, CsrUint8 *obexData);
extern CsrBool    CsrBtObexGetTypeOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset, CsrUint16 *typeLength);
#ifdef CSR_BT_INSTALL_OBEX_UTIL_IMG_OFFSET
extern CsrBool    CsrBtObexGetImgDescriptorOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset, CsrUint16 *imgDescriptorLength);
extern CsrBool    CsrBtObexGetImgHandleOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset);
#endif
extern CsrBool    CsrBtObexGetAppParametersOffset(CsrBtObexSession session, CsrUint8 *obexPacket,
                                                  CsrUint16 obexPacketLength, CsrUint16 *offset, CsrUint16 *length);
#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_OBEX_EXTRACTION_LIB_H__ */

