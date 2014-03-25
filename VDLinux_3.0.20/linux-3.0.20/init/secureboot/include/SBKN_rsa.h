/**
 * \file	rsa.h
 * \brief	implementation of rsa encryption/decryption and signature/verifycation
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/11/20
 */


#ifndef _RSA_H
#define _RSA_H

//////////////////////////////////////////////////////////////////////////
// Include Header Files
//////////////////////////////////////////////////////////////////////////
#include <linux/time.h>
#include "SBKN_CC_API.h"


//////////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/*!	\brief	generate RSA Context
 * \return	pointer to the generated context
 * \n		NULL	if memory allocation is failed
 */
SDRM_RSAContext *SDRM_RSA_InitCrt(cc_u32 RSAKeyByteLen);


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
				    cc_u8* RSA_D_Data,   cc_u32 RSA_D_Len);


/*!	\brief	generate and set RSA parameters
 * \param	crt							[out]rsa context
 * \param	PaddingMethod				[out]padding method
 * \param	RSA_N_Data					[out]n value
 * \param	RSA_N_Len					[out]byte-length of n
 * \param	RSA_E_Data					[out]e value
 * \param	RSA_E_Len					[out]byte-length of e
 * \param	RSA_D_Data					[out]d value
 * \param	RSA_D_Len					[out]byte-length of d
 * \return	CRYPTO_SUCCESS				if no error is occured
 * \n		CRYPTO_NULL_POINTER			if an argument is a null pointer
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if memory allocation is failed
 */
int SDRM_RSA_GenerateKey(CryptoCoreContainer *crt, cc_u32 PaddingMethod,
						 cc_u8* RSA_N_Data,   cc_u32 *RSA_N_Len,
						 cc_u8* RSA_E_Data,   cc_u32 *RSA_E_Len,
						 cc_u8* RSA_D_Data,   cc_u32 *RSA_D_Len);


/*!	\brief	generate and set RSA parameters with specfied e
 * \param	crt							[in/out]rsa context
 * \param	PaddingMethod				[in]padding method
 * \param	RSA_E_Data					[in]e value
 * \param	RSA_E_Len					[in]byte-length of e
 * \param	RSA_N_Data					[out]n value
 * \param	RSA_N_Len					[out]byte-length of n
 * \param	RSA_D_Data					[out]d value
 * \param	RSA_D_Len					[out]byte-length of d
 * \return	CRYPTO_SUCCESS				if no error is occured
 * \n		CRYPTO_NULL_POINTER			if an argument is a null pointer
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if memory allocation is failed
 */
int SDRM_RSA_GenerateND(CryptoCoreContainer *crt, cc_u32 PaddingMethod,
						 cc_u8* RSA_E_Data,   cc_u32 RSA_E_Len,
						 cc_u8* RSA_N_Data,   cc_u32 *RSA_N_Len,
						 cc_u8* RSA_D_Data,   cc_u32 *RSA_D_Len);


/*!	\brief	generate and set RSA parameters
 * \param	crt							[in]rsa context
 * \param	in							[in]message to encrypt
 * \param	inLen						[in]byte-length of in
 * \param	out							[out]encrypted message
 * \param	outLen						[out]byte-length of out
 * \return	CRYPTO_SUCCESS				if no error is occured
 * \n		CRYPTO_NULL_POINTER			if an argument is a null pointer
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if memory allocation is failed
 */
int SDRM_RSA_encrypt(CryptoCoreContainer *crt, cc_u8 *in, cc_u32 inLen, cc_u8 *out, cc_u32 *outLen);


/*!	\brief	RSA Decryption
 * \param	crt							[in]rsa context
 * \param	in							[in]message to decrypt
 * \param	inLen						[in]byte-length of in
 * \param	out							[out]decrypted message
 * \param	outLen						[out]byte-length of out
 * \return	CRYPTO_SUCCESS				if no error is occured
 * \n		CRYPTO_NULL_POINTER			if an argument is a null pointer
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if memory allocation is failed
 */
int SDRM_RSA_decrypt(CryptoCoreContainer *crt, cc_u8 *in, cc_u32 inLen, cc_u8 *out, cc_u32 *outLen);


/*!	\brief	generate signature for given value
 * \param	crt					[in]crypto env structure
 * \param	hash				[in]hash value
 * \param	hashLen				[in]byte-length of hash
 * \param	signature			[out]generated signature
 * \param	signLen				[out]byte-length of signature
 * \return	CRYPTO_SUCCESS		if success
 * \n		CRYPTO_NULL_POINTER	if given argument is a null pointer
 */
int SDRM_RSA_sign(CryptoCoreContainer *crt, cc_u8 *hash, cc_u32 hashLen, cc_u8 *signature, cc_u32 *signLen);


/*!	\brief	generate signature for given value
 * \param	crt						[in]crypto env structure
 * \param	hash					[in]hash value
 * \param	hashLen					[in]byte-length of hash
 * \param	signature				[in]signature
 * \param	signLen					[in]byte-length of signature
 * \param	result					[in]result of veryfing signature
 * \return	CRYPTO_SUCCESS			if success
 * \n		CRYPTO_NULL_POINTER		if given argument is a null pointer
 * \n		CRYPTO_INVALID_ARGUMENT	if the length of signature is invalid
 */
int SDRM_RSA_verify(CryptoCoreContainer *crt, cc_u8 *hash, cc_u32 hashLen, cc_u8 *signature, cc_u32 signLen, int *result);


#ifdef __cplusplus
}
#endif

#endif

/***************************** End of File *****************************/
