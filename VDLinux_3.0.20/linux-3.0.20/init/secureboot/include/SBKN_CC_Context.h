/**
 * \file	CC_Context.h
 * \brief	context definitions for samsung Crypto Library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon, Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/11/07
 */

#ifndef _DRM_CONTEXT_H
#define _DRM_CONTEXT_H

#include "SBKN_CC_Type.h"


////////////////////////////////////////////////////////////////////////////
// constant & context for Big Number Operation
////////////////////////////////////////////////////////////////////////////
/**
 * \brief	Big number structure
 *
 * used for big number representation
 */
typedef struct{						
	cc_u32 sign;						/**<	0 for positive, 1 for negative number	*/
	cc_u32 Length;						/**<	number of valid integers				*/
	cc_u32 Size;						/**<	unsigned long size of allocated memory	*/
	cc_u32 *pData;						/**<	unsigned long array					*/
}SDRM_BIG_NUM;

/**
 * \brief	Parameter sturcture
 *
 * Montgomery 알고리즘을 사용하기 위해 사용하는 Parameter structure
 */
typedef struct{							/**<	Structure to keep parameters for Montgomery	*/
	cc_u32	ri;							/**<	Length of Modulus							*/
	SDRM_BIG_NUM	*R;					/**<	R^2 mod m									*/
	SDRM_BIG_NUM	*Mod;				/**<	modulus										*/
	SDRM_BIG_NUM	*Inv_Mod;			/**<	Inverse of Modulus							*/
	cc_u32	N0;							/**<	m'											*/
}SDRM_BIG_MONT;

////////////////////////////////////////////////////////////////////////////
// constant & context for SHA-1
////////////////////////////////////////////////////////////////////////////
#define SDRM_SHA1_BLOCK_SIZ		20
#define SDRM_SHA1_DATA_SIZE		64
/**
 * \brief	SHA1 Context structure
 *
 * used for SHA1 parameters
 */
typedef struct{
	cc_u32 digest[SDRM_SHA1_BLOCK_SIZ / 4];		/**<	Message digest		*/
	cc_u32 countLo, countHi;					/**<	64-bit bit count	*/
	cc_u32 data[16];							/**<	SHS data buffer		*/
	int Endianness;
}SDRM_SHA1Context;

////////////////////////////////////////////////////////////////////
// constant & context for RSA
////////////////////////////////////////////////////////////////////////////
#define SDRM_RSA_BN_BUFSIZE		(RSA_KeyByteLen / 2 + 1)
#define SDRM_RSA_ALLOC_SIZE		(sizeof(SDRM_BIG_NUM) + SDRM_RSA_BN_BUFSIZE * SDRM_SIZE_OF_DWORD)

/**
 * \brief	RSA Context structure
 *
 * used for rsa parameters
 */
typedef struct{
	SDRM_BIG_NUM* n;					/**<	n value		*/
	SDRM_BIG_NUM* e;					/**<	public key	*/
	SDRM_BIG_NUM* d;					/**<	private key	*/

	cc_u32	k;							/**<	byte-length of n	*/
	cc_u32	pm;							/**<	padding method		*/
}SDRM_RSAContext;



#endif

/***************************** End of File *****************************/
