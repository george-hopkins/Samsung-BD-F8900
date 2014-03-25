/**
 * \file	rsa.c
 * \brief	implementation of rsa encryption/decryption and signature/verifycation
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/11/20
 * Note : Modified for support RSA-2048(Jisoon Park, 2007/03/14)
 */


////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
#include <linux/kernel.h>
#include "include/SBKN_rsa.h"
#include "include/SBKN_bignum.h"

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////


/*!	\brief	generate RSA Context
 * \return	pointer to the generated context
 * \n		NULL	if memory allocation is failed
 */
SDRM_RSAContext *SDRM_RSA_InitCrt(cc_u32 KeyByteLen) {
	SDRM_RSAContext *ctx;
	cc_u32			RSA_KeyByteLen = KeyByteLen;
	cc_u8			*pbBuf = (cc_u8*)kmalloc((sizeof(SDRM_RSAContext) + SDRM_RSA_ALLOC_SIZE * 3),GFP_KERNEL);

	if (pbBuf == NULL) {
		return NULL;
	}

	ctx		= (SDRM_RSAContext*)(void*)pbBuf;
	ctx->n	= SDRM_BN_Alloc((cc_u8*)ctx	   + sizeof(SDRM_RSAContext),   SDRM_RSA_BN_BUFSIZE);
	ctx->e	= SDRM_BN_Alloc((cc_u8*)ctx->n  + SDRM_RSA_ALLOC_SIZE,		SDRM_RSA_BN_BUFSIZE);
	ctx->d	= SDRM_BN_Alloc((cc_u8*)ctx->e  + SDRM_RSA_ALLOC_SIZE,		SDRM_RSA_BN_BUFSIZE);

	ctx->k	= RSA_KeyByteLen;

	return ctx;
}


/*!	\brief	set RSA parameters
 * \param	crt					[out]rsa context
 * \param	PaddingMethod		[in]padding method
 * \param	RSA_N_Data			[in]n value
 * \param	RSA_N_Len			[in]byte-length of n
 * \param	RSA_E_Data			[in]e value
 * \param	RSA_E_Len			[in]byte-length of e
 * \param	RSA_D_Data			[in]d value
 * \param	RSA_D_Len			[in]byte-length of d
 * \return	CRYPTO_SUCCESS		if no error is occured
 * \n		CRYPTO_NULL_POINTER	if an argument is a null pointer
 */
int SDRM_RSA_setNED(CryptoCoreContainer *crt, cc_u32 PaddingMethod,
				    cc_u8* RSA_N_Data,   cc_u32 RSA_N_Len,
				    cc_u8* RSA_E_Data,   cc_u32 RSA_E_Len,
				    cc_u8* RSA_D_Data,   cc_u32 RSA_D_Len) {
	if (crt == NULL || crt->ctx == NULL || crt->ctx->rsactx == NULL || RSA_N_Data == NULL) {
		return CRYPTO_NULL_POINTER;
	}
	SDRM_OS2BN(RSA_N_Data, RSA_N_Len, crt->ctx->rsactx->n);
	SDRM_BN_OPTIMIZE_LENGTH(crt->ctx->rsactx->n);
	if (RSA_E_Data != NULL) {
		SDRM_OS2BN(RSA_E_Data, RSA_E_Len, crt->ctx->rsactx->e);
		SDRM_BN_OPTIMIZE_LENGTH(crt->ctx->rsactx->e);
	}
	if (RSA_D_Data != NULL) {
		SDRM_OS2BN(RSA_D_Data, RSA_D_Len, crt->ctx->rsactx->d);
		SDRM_BN_OPTIMIZE_LENGTH(crt->ctx->rsactx->d);
	}
	crt->ctx->rsactx->pm = PaddingMethod;
	return CRYPTO_SUCCESS;
}


/*!	\brief	generate signature for given value
 * \param	crt						[in]crypto env structure
 * \param	hash					[in]hash value
 * \param	hashLen					[in]byte-length of hash
 * \param	signature				[in]signature
 * \param	signLen					[in]byte-length of signature
 * \param	result					[in]result of verifying signature
 * \return	CRYPTO_SUCCESS			if success
 * \n		CRYPTO_NULL_POINTER		if given argument is a null pointer
 * \n		CRYPTO_INVALID_ARGUMENT	if the length of signature is invalid
 */
int SDRM_RSA_verify(CryptoCoreContainer *crt, cc_u8 *hash, cc_u32 hashLen, cc_u8 *signature, cc_u32 signLen, int *result) {
	SDRM_BIG_NUM	*BN_dMsg, *BN_Sign;
	int				k, retVal;

	if (crt == NULL || crt->ctx == NULL || crt->ctx->rsactx == NULL) {
		return CRYPTO_NULL_POINTER;
	}

	cc_u32          RSA_KeyByteLen = crt->ctx->rsactx->k;
	cc_u8			*pbBuf = (cc_u8*)kmalloc((SDRM_RSA_ALLOC_SIZE * 3),GFP_KERNEL);

	if (pbBuf == NULL) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	if (crt == NULL || crt->ctx == NULL || crt->ctx->rsactx == NULL || hash == NULL || signature == NULL) {
		kfree(pbBuf);
		return CRYPTO_NULL_POINTER;
	}

	if (hashLen > crt->ctx->rsactx->k) {
		kfree(pbBuf);
		return CRYPTO_MSG_TOO_LONG;
	}

	BN_dMsg	  = SDRM_BN_Alloc((cc_u8*)pbBuf,					  SDRM_RSA_BN_BUFSIZE);
	BN_Sign	  = SDRM_BN_Alloc((cc_u8*)BN_dMsg + SDRM_RSA_ALLOC_SIZE, SDRM_RSA_BN_BUFSIZE);

	k = crt->ctx->rsactx->k;

	SDRM_OS2BN(signature, signLen, BN_Sign);
//	SDRM_PrintBN("Generated Sign : ", BN_Sign);
	
	//RSA Verification by modular exponent
	retVal = SDRM_BN_ModExp(BN_dMsg, BN_Sign, crt->ctx->rsactx->e, crt->ctx->rsactx->n);
	if (retVal != CRYPTO_SUCCESS) {
		kfree(pbBuf);

		return retVal;
	}

	//Msg Depadding
	switch(crt->ctx->rsactx->pm) {
	case ID_NO_PADDING :
		SDRM_OS2BN(hash, hashLen, BN_Sign);
		if (SDRM_BN_Cmp(BN_Sign, BN_dMsg) == 0)
		{
			*result = CRYPTO_VALID_SIGN;
		}
		else
		{
			*result = CRYPTO_INVALID_SIGN;
		}

	default :
		break;
	}

	SDRM_BN_FREE(pbBuf);

	return CRYPTO_SUCCESS;
}

/***************************** End of File *****************************/
