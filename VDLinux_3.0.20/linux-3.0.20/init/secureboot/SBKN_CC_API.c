/**
 * \file	CC_API.c
 * \brief	API of samsung Crypto Library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jae Heung Lee
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/10/24
 * Note : modified for implementation, by Jisoon, Park, 06/11/06
 */


////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
//#include <linux/stdlib.h>
#include "include/SBKN_CryptoCore.h"
#include "include/SBKN_hash.h"
#include "include/SBKN_rsa.h"


////////////////////////////////////////////////////////////////////////////
// global log context for crypto library
////////////////////////////////////////////////////////////////////////////
//Log4DRM_CTX ECRYPTO_API CryptoLogCTX;


////////////////////////////////////////////////////////////////////////////
// functions
////////////////////////////////////////////////////////////////////////////

void *CCMalloc(int siz) {
	cc_u8 *pbBuf = (cc_u8*)kmalloc(siz,GFP_KERNEL);

	if (pbBuf == NULL) {
		return NULL;
	} else {
		memset(pbBuf, 0, siz);
		return (void*)pbBuf;
	}
}


/*!	\brief	memory allocation and initialize the crypt sturcture
 * \param	algorithm	[in]algorithm want to use
 * \return	address of created sturcture
 */
CryptoCoreContainer *create_CryptoCoreContainer(cc_u32 algorithm)
{
	CryptoCoreContainer *crt;
	
	// allocate memory for crypt data structure (by using CCMalloc)
	crt = (CryptoCoreContainer *)CCMalloc(sizeof(CryptoCoreContainer));
	crt->ctx = (CryptoCoreCTX *)CCMalloc(sizeof(CryptoCoreCTX));
	
	crt->MD_init			= NULL;
	crt->MD_update			= NULL;
	crt->MD_final			= NULL;
	crt->MD_getHASH			= NULL;
	crt->DS_sign			= NULL;
	crt->DS_verify			= NULL;
	crt->RSA_genKeypair		= NULL;
	crt->RSA_genKeypairWithE= NULL;
	crt->RSA_setKeypair		= NULL;

	// allocate memory for context data structure
	// and set up the member functions according to the algorithm
	crt->alg = algorithm;
	switch(algorithm) {
	case ID_SHA1:
//		crt->ctx->sha1ctx			= (SDRM_SHA1Context*)CCMalloc(sizeof(SDRM_SHA1Context));
//		crt->MD_init				= SDRM_SHA1_init;
//		crt->MD_update				= SDRM_SHA1_update;
//		crt->MD_final				= SDRM_SHA1_final;
//		crt->MD_getHASH				= SDRM_SHA1_hash;
		break;
	case ID_RSA:
		/*fall through*/
	case ID_RSA1024:
		crt->ctx->rsactx			= SDRM_RSA_InitCrt(128);
#ifdef USE_PRIVATE_KEY	
		crt->RSA_genKeypair			= SDRM_RSA_GenerateKey;
		crt->RSA_genKeypairWithE	= SDRM_RSA_GenerateND;
		crt->DS_sign				= SDRM_RSA_sign;
#endif		
		crt->RSA_setKeypair			= SDRM_RSA_setNED;
		crt->DS_verify				= SDRM_RSA_verify;
		break;
	case ID_RSA2048:
		crt->ctx->rsactx			= SDRM_RSA_InitCrt(256);
#ifdef USE_PRIVATE_KEY	
		crt->RSA_genKeypair			= SDRM_RSA_GenerateKey;
		crt->RSA_genKeypairWithE	= SDRM_RSA_GenerateND;
		crt->DS_sign				= SDRM_RSA_sign;
#endif		
		crt->RSA_setKeypair			= SDRM_RSA_setNED;
		crt->DS_verify				= SDRM_RSA_verify;
		break;
	}
	return crt;
}


/*!	\brief	free allocated memory
 * \param	crt		[in]crypt context
 * \return	void
 */
void destroy_CryptoCoreContainer(CryptoCoreContainer* crt)
{
	// free context data structure
	switch(crt->alg) {
	case ID_SHA1:
		kfree(crt->ctx->sha1ctx);
		break;
	case ID_RSA:
	case ID_RSA1024:
	case ID_RSA2048:
		kfree(crt->ctx->rsactx);
		break;
	}
	
	// free CryptoCoreContainer data structure
	kfree(crt->ctx);
	kfree(crt);
}

/***************************** End of File *****************************/
