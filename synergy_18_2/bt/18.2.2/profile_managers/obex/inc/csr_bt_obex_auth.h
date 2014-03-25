#ifndef CSR_BT_OBEX_AUTH_H__
#define CSR_BT_OBEX_AUTH_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#define    CSR_BT_NONCE_LEN                (16)        /*    Length in bytes of the nonce */
#define    CSR_BT_DIGEST_LEN               (16)        /*    Length in bytes of the digest */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *
 *    Function:    CsrBtGetNonce
 *
 *    Parameters:    returns calculated nonce in indicated variable
 *
 *    Description:Generate a 16 byte nonce based on the current time. This function
 *                assumes that the nonce variable is already allocated and contains
 *                sufficient memory for the nonce.
 *
 ****************************************************************************/
#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
void CsrBtGetNonce(CsrUint8 *nonce);

/****************************************************************************
 *
 *    Function:    GetDigest
 *
 *    Parameters:    nonce and passkey which will be used as input to the digest
 *                generator. It is assumed that the nonce is always 16 bytes
 *                long. The length of the passkey is variable and must be
 *                included.
 *                Returns the calculated digest.
 *
 *    Description:Generate a 16 byte digest based on the nonce and passkey. This
 *                function assumes that the nonce variable is already allocated and
 *                contains sufficient memory for the digest.
 *
 ****************************************************************************/
void CsrBtGetDigest(CsrUint8 *nonce, CsrUint8 *passkey, CsrUint16 passkeyLen, CsrUint8 *digest);
/****************************************************************************
 *
 *    Function:    CsrBtDigestValidate
 *
 *    Parameters:    nonce and passkey which will be used as input to the digest
 *                generator. It is assumed that the nonce is always 16 bytes
 *                long. The length of the passkey is variable and must be
 *                included. The received digest must be included and is assumed
 *                to be of length 16 byte allways.
 *                Returns TRUE if calculated digest equals received digest.
 *
 *    Description:Generate a 16 byte digest based on the nonce and passkey. Compare
 *                the received digest with the just created one.
 *
 ****************************************************************************/
CsrBool CsrBtDigestValidate(CsrUint8 *nonce, CsrUint8 *passkey, CsrUint16 passkeyLen, CsrUint8 *recvDigest);
#endif
#ifdef __cplusplus
}
#endif

#endif
