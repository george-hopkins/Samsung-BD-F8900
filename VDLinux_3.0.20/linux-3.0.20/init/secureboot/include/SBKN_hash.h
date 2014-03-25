/**
 * \file	hash.h
 * \brief	hash API function
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/11/08
 */

#ifndef _HASH_H
#define _HASH_H

////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
#include "SBKN_CC_API.h"


////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/*!	\brief	initialize CryptoCoreContainer context
 * \param	crt					[out]CryptoCoreContainer context
 * \return	CRYPTO_SUCCESS		if no error is occured
 * \n		CRYPTO_NULL_POINTER	if given argument is a null pointer
 */
int SDRM_SHA1_init(CryptoCoreContainer *crt);


/*!	\brief	process a message block
 * \param	crt					[out]CryptoCoreContainer context
 * \param	msg					[in]message
 * \param	msglen				[in]byte-length of msg
 * \return	CRYPTO_SUCCESS		if no error is occured
 * \n		CRYPTO_NULL_POINTER	if given argument is a null pointer
 */
int SDRM_SHA1_update(CryptoCoreContainer *crt, cc_u8 *msg, cc_u32 msglen);


/*!	\brief	get hashed message
 * \param	crt					[in]CryptoCoreContainer context
 * \param	output				[out]hashed message
 * \return	CRYPTO_SUCCESS		if no error is occured
 * \n		CRYPTO_NULL_POINTER	if given argument is a null pointer
 */
int SDRM_SHA1_final(CryptoCoreContainer *crt, cc_u8 *output);


/*!	\brief	get hashed message from message
 * \param	crt					[in]CryptoCoreContainer context
 * \param	msg					[in]message
 * \param	msglen				[in]byte-length of msg
 * \param	output				[out]hashed message
 * \return	CRYPTO_SUCCESS		if no error is occured
 * \n		CRYPTO_NULL_POINTER	if given argument is a null pointer
 */
int SDRM_SHA1_hash(CryptoCoreContainer *crt, cc_u8 *msg, cc_u32 msglen, cc_u8 *output);

#ifdef __cplusplus
}
#endif

#endif

/***************************** End of File *****************************/
