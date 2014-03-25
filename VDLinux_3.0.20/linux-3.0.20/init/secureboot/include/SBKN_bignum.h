/**
 * \file	bignum.h
 * \brief	big number library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/08/03
 */


#ifndef _BIGNUM_H
#define _BIGNUM_H


////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
#include "SBKN_CryptoCore.h"


////////////////////////////////////////////////////////////////////////////
// Parameters and Bit-wise Macros
////////////////////////////////////////////////////////////////////////////
/*!	\brief	byte-length of single cc_u32	*/
#define SDRM_SIZE_OF_DWORD			4
/*!	\brief	bit-length of single cc_u32	*/
#define SDRM_BitsInDWORD			(8 * SDRM_SIZE_OF_DWORD)

/*!	\brief	get k-th bit form cc_u32 array A	*/
#define SDRM_CheckBitUINT32(A, k)	(0x01 & ((A)[(k) >> 5] >> ((k) & 31)))

/*!	\brief	get k-th byte from cc_u32 array A	*/
#define SDRM_CheckByteUINT32(A, k)	(cc_u8)(0xff & (A[(k) >> 2] >> (((k) & 3 ) << 3)))
#define SDRM_isEven0(X)				(((X)[0] & 0x01) == 0)
#define SDRM_isOdd0(X)				(((X)[0] & 0x01) == 1)

/*!	\brief	increase 1 from Byte Array A, byte-length of B	*/
#define SDRM_INC_BA(A, B)			do {											\
										for (i = 0; i < (B); i++) {					\
											if (++A[i] != 0) break;					\
										}											\
									} while(0)										\

  
////////////////////////////////////////////////////////////////////////////
// MACROs for cc_u32 Evaluation
////////////////////////////////////////////////////////////////////////////
/*!	\brief	Double-width UINT32 Multiplication
 * \n	Dest		[out]destination, 2-cc_u32-size array
 * \n	Src1		[in]first element
 * \n	Src2		[in]second element
 * \return	void
 */
#ifndef _OP64_NOTSUPPORTED
#define SDRM_DIGIT_Mul(Dest, Src1, Src2)	do {																		\
												(Dest)[0] = (cc_u32) ((cc_u64)(Src1) * (Src2));							\
												(Dest)[1] = (cc_u32)(((cc_u64)(Src1) * (Src2)) >> SDRM_BitsInDWORD);	\
											} while(0)
#else
void SDRM_DIGIT_Mul(cc_u32 *Dest, cc_u32 Src1, cc_u32 Src2);
#endif
	

/*!	\brief	Doublue-width DWROD Division
 * \n	Src1		[in]upper-digit of dividend
 * \n	Src2		[in]lower-digit of dividend
 * \n	Div			[in]divisor
 */
#ifndef _OP64_NOTSUPPORTED
cc_u32 SDRM_DIGIT_Div(cc_u32 Src1, cc_u32 Src2, cc_u32 Div);
//#define SDRM_DIGIT_Div(Src1, Src2, Div)	(cc_u32)((((cc_u64)(Src1) << SDRM_BitsInDWORD) ^ (Src2)) / (Div))
#else
cc_u32 SDRM_DIGIT_Div(cc_u32 Src1, cc_u32 Src2, cc_u32 Div);
#endif

/*!	\brief	Doublue-width DWROD Modular
 * \n	Src1		[in]upper-digit of dividend
 * \n	Src2		[in]lower-digit of dividend
 * \n	Div			[in]divisor
 */
#ifndef _OP64_NOTSUPPORTED
#define SDRM_DIGIT_Mod(Src1, Src2, Div)	(cc_u32)((((cc_u64)(Src1) << SDRM_BitsInDWORD) ^ (Src2)) % (Div))
#else
cc_u32 SDRM_DIGIT_Mod(cc_u32 Src1, cc_u32 Src2, cc_u32 Div);
#endif

/*!	\brief	Copy Digit Array
 * \n	Dest		[in]destination, cc_u32 array
 * \n	Src			[in]source, cc_u32 array
 * \n	Size		[in]length of dSrc
 */
#define SDRM_DWD_Copy(Dest, Src, Size)	do {												\
											memcpy(Dest, Src, SDRM_SIZE_OF_DWORD * Size);	\
										} while(0)

	
////////////////////////////////////////////////////////////////////////////
// MACROs for Big Number
////////////////////////////////////////////////////////////////////////////
/*!	\brief	check big number a is an odd number	*/
#define SDRM_BN_IS_ODD(a)			((a)->pData[0] & 1)

/*!	\brief	free allocated memory	*/
#define SDRM_BN_FREE(X)				do {if (X) kfree(X);} while(0)

/*!	\brief	optimize SDRM_BIG_NUM's length member	*/
#define SDRM_BN_OPTIMIZE_LENGTH(BN)	do {															\
										while((BN)->Length > 0)										\
											if((BN)->pData[(BN)->Length - 1])						\
												break;												\
											else													\
												(BN)->Length--;										\
									} while(0)

/*!	\brief	check big number's sign	*/
#define SDRM_IS_BN_NEGATIVE(X)		((X)->sign)

/*!	\brief	calc cc_u32-length when byte array is converted to cc_u32 array	*/
#define SDRM_B2DLEN(X)				((X) > 0 ? (((X) - 1) >> 2) + 1 : 0)

/*!	\brief	count byte-length of big number	*/
#define	SDRM_BN_GETBYTELEN(X, A)	do {																		\
										if (!((X)->Length)) (A) = 0;											\
										else {																	\
											(A) = (X)->Length * 4;												\
											while(SDRM_CheckByteUINT32((X)->pData, (A) - 1) == 0) {(A) -= 1;}	\
										}																		\
									} while(0)



////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////
/*!	\brief	some special big numbers	*/
extern SDRM_BIG_NUM *BN_Zero, *BN_One;


////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

int SDRM_DWD_Classical_REDC(cc_u32 *pdDest, cc_u32 DstLen, cc_u32 *pdModulus, cc_u32 ModLen);

/*!	\brief	Convert Big Number to Octet String
 * \param	BN_Src	[in]source integer
 * \param	dDstLen	[in]Byte-length of pbDst
 * \param	pbDst	[out]output octet string
 * \return	CRYPTO_SUCCESS	if no error is occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if arrary is too small
 */
int	SDRM_BN2OS(SDRM_BIG_NUM *BN_Src, cc_u32 dDstLen, cc_u8 *pbDst);


/*!	\brief	Convert Octet String to Big Number
 * \param	pbSrc	[in]source octet string
 * \param	dSrcLen	[in]Byte-length of pbSrc
 * \param	BN_Dst	[out]output big number
 * \return	CRYPTO_SUCCESS	if no error is occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if arrary is too small
 */
int	SDRM_OS2BN(cc_u8* pbSrc, cc_u32 dSrcLen, SDRM_BIG_NUM *BN_Dst);


/*!	\brief	Converts a nonnonegative integer to an octet string of a specified length
 * \param	BN_Src					[in]nonnegative integer to be converted
 * \param	dDstLen					[in]intended length of the resulting octet string
 * \param	pbDst					[out]corresponding octet string of length dDstLen
 * \return	CRYPTO_SUCCESS			if no error is occured
 */
int	SDRM_I2OSP(SDRM_BIG_NUM *BN_Src, cc_u32 dDstLen, cc_u8 *pbDst);


/*!	\brief	Clear the SDRM_BIG_NUM structure
 * \param	BN_Src		[in]source
 * \return	CRYPTO_SUCCESS
 */
int SDRM_BN_Clr(SDRM_BIG_NUM* BN_Src);


/*!	\brief	copy SDRM_BIG_NUM
 * \param	BN_Dest		[out]destination
 * \param	BN_Src		[in]source
 * \return	CRYPTO_SUCCESS
 */
int SDRM_BN_Copy(SDRM_BIG_NUM* BN_Dest, SDRM_BIG_NUM* BN_Src);


/*!	\brief	allocate big number from buffer
 * \param	pbSrc		[in]start pointer of buffer
 * \param	dSize		[in]buffer size of big number
 * \return	pointer of SDRM_BIG_NUM structure
 */
SDRM_BIG_NUM *SDRM_BN_Alloc(cc_u8* pbSrc, cc_u32 dSize);


/*!	\brief	Allocate a new big number object
 * \param	dSize		[in]buffer size of big number
 * \return	pointer of SDRM_BIG_NUM structure
 * \n		NULL if memory allocation is failed
 */
SDRM_BIG_NUM *SDRM_BN_Init(cc_u32 dSize);


/*!	\brief	Compare two Big Number
 * \param	BN_Src1		[in]first element
 * \param	BN_Src2		[in]second element
 * \return	1 if BN_Src1 is larger than pdSrc2
 * \n		0 if same
 * \n		-1 if BN_Src2 is larger than pdSrc1
 */
int SDRM_BN_Cmp(SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2);			// sign 비교안함


/*!	\brief	Compare two Big Number considering sign
 * \param	BN_Src1		[in]first element
 * \param	BN_Src2		[in]second element
 * \return	1 if BN_Src1 is larger than pdSrc2
 * \n		0 if same
 * \n		-1 if BN_Src2 is larger than pdSrc1
 */
int SDRM_BN_Cmp_sign(SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2);		// sign 비교


/*!	\brief	Generate simple random number
 * \param	BN_Dst		[out]destination
 * \param	BitLen		[in]bit-length of generated random number
 * \return	CRYPTO_SUCCESS if no error is occured
 */
int SDRM_BN_Rand(SDRM_BIG_NUM *BN_Dst, cc_u32 BitLen);


/*!	\brief	Big Number Shift Left
 * \param	BN_Dst			[out]destination
 * \param	BN_Src			[in]source
 * \param	NumOfShift		[in]shift amount
 * \return	CRYPTO_SUCCESS	if no error occured
 */
int SDRM_BN_SHL(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, cc_u32 NumOfShift);


/*!	\brief	Big Number Shift Right
 * \param	BN_Dst			[out]destination
 * \param	BN_Src			[in]source
 * \param	NumOfShift		[in]shift amount
 * \return	CRYPTO_SUCCESS	if no error occured
 */
int SDRM_BN_SHR(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, cc_u32 NumOfShift);


/*!	\brief	Big Number Addition
 * \param	BN_Dst						[out]destination
 * \param	BN_Src1						[in]first element
 * \param	BN_Src2						[in]second element
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Add(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2);


/*!	\brief	Big Number Subtraction
 * \param	BN_Dst						[out]destination
 * \param	BN_Src1						[in]first element
 * \param	BN_Src2						[in]second element
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Sub(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2);


/*!	\brief	Big Number Multiplication
 * \param	BN_Dst						[out]destination
 * \param	BN_Src1						[in]first element
 * \param	BN_Src2						[in]second element
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Mul(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2);


/*!	\brief	Big Number Division
 * \param	BN_Quotient					[out]quotient
 * \param	BN_Remainder				[out]remainder
 * \param	BN_Dividend					[in]dividend
 * \param	BN_Divisor					[in]divisor
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Div(SDRM_BIG_NUM *BN_Quotient, SDRM_BIG_NUM *BN_Remainder, SDRM_BIG_NUM *BN_Dividend, SDRM_BIG_NUM *BN_Divisor);


/*!	\brief	Big Number Modular Addition
 * \param	BN_Dst						[out]destination
 * \param	BN_Src1						[in]first element of addition
 * \param	BN_Src2						[in]second element of addition
 * \param	BN_Modulus					[in]modular m
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_ModAdd(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2, SDRM_BIG_NUM *BN_Modulus);


/*!	\brief	Big Number Modular Subtraction
 * \param	BN_Dst						[out]destination
 * \param	BN_Src1						[in]first element of subtraction
 * \param	BN_Src2						[in]second element of subtraction
 * \param	BN_Modulus					[in]modular m
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_ModSub(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2, SDRM_BIG_NUM *BN_Modulus);


/*!	\brief	Big Number Modular Reduction
 * \param	BN_Dst						[out]destination
 * \param	BN_Src						[in]source
 * \param	BN_Modulus					[in]modular m
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_ModRed(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, SDRM_BIG_NUM *BN_Modulus);


/*!	\brief	Big Number Modular Multiplication
 * \param	BN_Res						[out]destination
 * \param	BN_Src1						[in]first element of multiplication
 * \param	BN_Src2						[in]second element of multipliation
 * \param	BN_Modulus					[in]modular m
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_ModMul(SDRM_BIG_NUM *BN_Res, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2, SDRM_BIG_NUM *BN_Modulus);


/*!	\brief	Big Number Modular Inverse
 * \param	BN_Dst						[out]destination
 * \param	BN_Src						[in]soure
 * \param	BN_Modulus					[in]modular m
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 * \n		CRYPTO_NEGATIVE_INPUT		if source is negative value
 * \n		CRYPTO_INVERSE_NOT_EXIST	if inverse is not exists
 */
int SDRM_BN_ModInv(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, SDRM_BIG_NUM *BN_Modulus);


/*!	\brief	Big Number Modular Exponentiation
 * \param	BN_Dst						[out]destination
 * \param	BN_Base						[in]base
 * \param	BN_Exponent					[in]exponent
 * \param	BN_Modulus					[in]modular m
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 * \n		CRYPTO_ERROR				if evaluation is failed
 */
int SDRM_BN_ModExp(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Base, SDRM_BIG_NUM *BN_Exponent, SDRM_BIG_NUM *BN_Modulus);

/*!	\brief	Big Number Modular Exponentiation2 - Karen's method
 * \param	BN_Dst		[out]destination
 * \param	BN_Base		[in]base
 * \param	BN_Exponent	[in]exponent
 * \param	BN_Modulus	[in]modular m
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 * \n		CRYPTO_ERROR	if evaluation is failed
 */
int SDRM_BN_ModExp2(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Base, SDRM_BIG_NUM *BN_Exponent, SDRM_BIG_NUM *BN_Modulus);

#ifdef NOT_ONLY_RSA_VERIFY
/*!	\brief	Show out a Big Number
 * \param	level		[in]log level
 * \param	s			[in]title
 * \param	bn			[in]big number to show out
 * \return	void
 */
void SDRM_PrintBN(const char* s, SDRM_BIG_NUM* bn);
#endif

/*!	\brief	Calc bit-length of Big Number
 * \param	BN_Src	[in]source
 * \return	bit-length
 */
int SDRM_BN_num_bits(SDRM_BIG_NUM *BN_Src);


/*!	\brief	Calc bit-length of cc_u32
 * \param	pdSrc	[in]source
 * \return	bit-length
 */
int	SDRM_UINT32_num_bits(cc_u32 *pdSrc);


/*!	\brief	Calc bit-length of integer
 * \param	Src	[in]source
 * \return	bit-length
 */
int	SDRM_INT_num_bits(int Src);


/*!	\brief	Montgomery Multiplication
 * \param	BN_Dst		[out]destination, montgomery number
 * \param	BN_Src1		[in]first element, montgomery number
 * \param	BN_Src2		[in]second element, montgomery number
 * \param	Mont		[in]montgomery parameters
 * \return	CRYPTO_SUCCESS if no error occured
 */
int SDRM_MONT_Mul(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2, SDRM_BIG_MONT *Mont);


/*!	\brief	Convert normal number to Montgomery number
 * \n	Dst			[out]destination, montgomery number
 * \n	SRC1		[in]source, normal number
 * \n	MONT		[in]montgomery parameters
 * \return	CRYPTO_SUCCESS if no error occured
 */
#define SDRM_MONT_Zn2rzn(DST, SRC1, MONT)	SDRM_MONT_Mul(DST, SRC1, (MONT)->R, MONT)


/*!	\brief	Convert Montgomery number to normal number
 * \param	BN_Dst		[out]destination, normal number
 * \param	BN_Src1		[in]source, montgomery number
 * \param	Mont		[in]montgomery parameters
 * \return	CRYPTO_SUCCESS if no error occured
 */
int SDRM_MONT_Rzn2zn(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_MONT *Mont);


/*!	\brief	Allocate new momory for Montgomery parameter
 * \param	dSize	[in]size of buffer of big number
 * \return	Pointer to created structure
 * \n		NULL if malloc failed
 */
SDRM_BIG_MONT *SDRM_MONT_Init(cc_u32 dSize);


/*!	\brief	Set Montgomery parameters
 * \param	Mont		[out]montgomery parameter
 * \param	BN_Modulus	[in]modular m
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		BN_NOT_ENOUGHT_BUFFER if malloc is failed
 * \n		CRYPTO_INVERSE_NOT_EXIST if inverse is not exists
 */
int SDRM_MONT_Set(SDRM_BIG_MONT *Mont, SDRM_BIG_NUM *BN_Modulus);


/*!	\brief	Free allocated memory for montgomery paramter
 * \param	Mont	[in]montgomery parameters
 * \return	void
 */
void SDRM_MONT_Free(SDRM_BIG_MONT *Mont);


/*!	\brief	get gcd of two big number
 * \param	BN_Src1						[in]first element
 * \param	BN_Src2						[in]second element
 * \return	CRYPTO_ISPRIME				if two elements are relatively prime
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if memory allocation is failed
 * \n		CRYPTO_ERROR	otherwise
 */
int SDRM_BN_CheckRelativelyPrime(SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2);


/*!	\brief	MILLER_RABIN Test
 * \param	n					[in]value to test
 * \param	t					[in]security parameter
 * \return	CRYPTO_ISPRIME			if n is (probably) prime
 * \n		CRYPTO_INVALID_ARGUMENT	if n is composite
 */
int SDRM_BN_MILLER_RABIN(SDRM_BIG_NUM* n, cc_u32 t);


#ifdef __cplusplus
}
#endif

#endif	//	_BIGNUM_H

/***************************** End of File *****************************/
