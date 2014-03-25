#ifndef CSR_BT_OBEX_LOOKUP_LIB_H__
#define CSR_BT_OBEX_LOOKUP_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_obex.h"
#include "csr_bt_obex_common.h"
#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes for lookup functions */
extern CsrUint16 CsrBtObexHeaderIndex(CsrBtObexSession session, CsrUint8 * data, CsrUint8 headerType, CsrUint16 * headerLength);
extern CsrUint16 CsrBtObexHeaderDwordIndex(CsrBtObexSession session, CsrUint8 * data, CsrUint8 headerType, CsrUint32 * headerLength);
extern CsrUint16 CsrBtObexNameHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *nameLength);
extern CsrUint16 CsrBtObexBodyHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *bodyLength);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
extern CsrUint16 CsrBtObexDescriptionHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *descriptionLength);
#endif
extern CsrUint16 CsrBtObexObjectLengthHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint32 *lengthOfObjectValue);
extern CsrUint16 CsrBtObexCountHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint32 *countValue);
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
extern CsrUint16 CsrBtObexTargetHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *targetLength);
#endif
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
extern CsrUint16 CsrBtObexConnectionIdHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint32 *connectionIdValue);
#endif
extern CsrUint16 CsrBtObexTypeHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *typeLength);
#ifdef CSR_BT_INSTALL_OBEX_UTIL_IMG_OFFSET
extern CsrUint16 CsrBtObexImgHandleHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *imgHandleLength);
extern CsrUint16 CsrBtObexImgDescriptorHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *imgDescriptorLength);
#endif
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
extern CsrBool CsrBtObexGetSingleResponseModeParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *srmValue);
extern CsrBool CsrBtObexGetSrmpParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *srmpValue);
extern CsrBool CsrBtObexGetPermissionsParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint32 *permissionsValue);
extern CsrBool CsrBtObexGetActionIdParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *actionIdValue);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
CsrUint16 CsrBtObexAppParamIndex(CsrUint8 *data);
#endif
#ifdef __cplusplus
}
#endif

#endif /* OBEX_LOOKUP_LIB_H_ */
