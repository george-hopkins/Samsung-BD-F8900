/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_md5.h"
#include "csr_bt_obex_auth.h"
#include "csr_bt_util.h"

#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
void CsrBtGetNonce(CsrUint8 *nonce)
{
    CsrUint32     tb;
    CsrUintFast16 i;
    CsrUint32     rndNumber;

    for (i = 0; i<(CSR_BT_NONCE_LEN/sizeof(CsrUint32)); i++)
    {
        tb = CsrTimeGet(NULL);
        /*    add a little math so the 4 bytes are not identical */
        rndNumber = (i+1) * (tb ^ (tb >> 1));
                nonce[i*4+3] = (CsrUint8)((rndNumber & 0x000000FF));
                nonce[i*4+2] =(CsrUint8)((rndNumber >> 8) & 0x000000FF);
                nonce[i*4+1] =(CsrUint8)((rndNumber >> 16) & 0x000000FF);
                nonce[i*4] =(CsrUint8)((rndNumber >> 24) & 0x000000FF);
    }
}

void CsrBtGetDigest(CsrUint8 *nonce, CsrUint8 *passkey, CsrUint16 passkeyLen, CsrUint8 *digest)
{
    md5_state_t state;

    /*    to do: add the passkey to the nonce before calculating the digest */
    md5_init(&state);
    md5_append(&state, (const md5_byte_t *) nonce, CSR_BT_NONCE_LEN);
    md5_append(&state, (CsrUint8*) ":", 1);
    md5_append(&state, passkey, passkeyLen);
    md5_finish(&state, (md5_byte_t*) digest);
}
CsrBool CsrBtDigestValidate(CsrUint8 *nonce, CsrUint8 *passkey, CsrUint16 passkeyLen, CsrUint8 *recvDigest)
{
    CsrUint8        myDigest[CSR_BT_DIGEST_LEN];

    CsrBtGetDigest(nonce, passkey, passkeyLen, myDigest);
    if ( CsrMemCmp(recvDigest, myDigest, CSR_BT_DIGEST_LEN) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
