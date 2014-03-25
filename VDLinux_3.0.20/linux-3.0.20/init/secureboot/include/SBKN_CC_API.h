/**
 * \file	CC_API.h
 * \brief	API of samsung Crypto Library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jae Heung Lee
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/11/06
 */

#ifndef _CRYPTOCORE_API_H
#define _CRYPTOCORE_API_H

#include "SBKN_CryptoCore.h"

typedef union
{
	SDRM_SHA1Context	*sha1ctx;				//Hash : SHA1 Hash Context
	SDRM_RSAContext		*rsactx;				//Asymmetric Encryption and Signature : RSA Context
} CryptoCoreCTX;


/**
 * \brief	Parameter sturcture
 *
 * Crypto Library를 쉽게 사용하기 위해  사용하는 Parameter structure
 */
typedef struct _CryptoCoreContainer
{
	int alg;																				/**<	Algorithm	*/
	CryptoCoreCTX *ctx;																		/**<	Algorithm	*/
	
	// Message Digest (MD5, SHA-1)
	int (*MD_init)    (struct _CryptoCoreContainer *crt);
	int (*MD_update)  (struct _CryptoCoreContainer *crt, cc_u8 *msg, cc_u32 msglen);
	int (*MD_final)   (struct _CryptoCoreContainer *crt, cc_u8 *output);
	int (*MD_getHASH) (struct _CryptoCoreContainer *crt, cc_u8 *msg, cc_u32 msglen, cc_u8 *output);
	
	// Digital Signature (DSA, EC-DSA)
	int (*DS_sign)    (struct _CryptoCoreContainer *crt, cc_u8 *hash, cc_u32 hashLen, cc_u8 *signature, cc_u32 *signLen);
	int (*DS_verify)  (struct _CryptoCoreContainer *crt, cc_u8 *hash, cc_u32 hashLen, cc_u8 *signature, cc_u32 signLen, int *result);

	// RSA Support Functions
	int (*RSA_genKeypair)(struct _CryptoCoreContainer *crt, cc_u32 PaddingMethod,
						  cc_u8* RSA_N_Data,   cc_u32 *RSA_N_Len,
						  cc_u8* RSA_E_Data,   cc_u32 *RSA_E_Len,
						  cc_u8* RSA_D_Data,   cc_u32 *RSA_D_Len);
	int (*RSA_genKeypairWithE)(struct _CryptoCoreContainer *crt, cc_u32 PaddingMethod,
						  cc_u8* RSA_E_Data,   cc_u32 RSA_E_Len,
						  cc_u8* RSA_N_Data,   cc_u32 *RSA_N_Len,
						  cc_u8* RSA_D_Data,   cc_u32 *RSA_D_Len);
	int (*RSA_setKeypair)(struct _CryptoCoreContainer *crt, cc_u32 PaddingMethod,
						  cc_u8* RSA_N_Data,   cc_u32 RSA_N_Len,
						  cc_u8* RSA_E_Data,   cc_u32 RSA_E_Len,
						  cc_u8* RSA_D_Data,   cc_u32 RSA_D_Len);

} CryptoCoreContainer;

#ifdef __cplusplus
extern "C" {
#endif


/*!	\brief	memory allocation and initialize the CryptoCoreContainer sturcture
 * \param	algorithm	[in]algorithm want to use
 * \return	address of created sturcture
 */
CryptoCoreContainer ECRYPTO_API *create_CryptoCoreContainer(cc_u32 algorithm);


/*!	\brief	free allocated memory
 * \param	crt		[in]CryptoCoreContainer context
 * \return	void
 */
void ECRYPTO_API destroy_CryptoCoreContainer(CryptoCoreContainer* crt);


void ECRYPTO_API *CCMalloc(int siz);

#ifdef __cplusplus
}
#endif

#endif

/***************************** End of File *****************************/
