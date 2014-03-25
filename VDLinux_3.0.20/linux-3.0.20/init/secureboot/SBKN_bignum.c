/**
 * \file	bignum.c
 * \brief	big number library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/08/03
 */


////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
#include "include/SBKN_bignum.h"



////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////
cc_u32			DWD_Zero[2]	= {0, 0};
cc_u32			DWD_One[2]	= {1, 0};

SDRM_BIG_NUM	_BN_Zero	= {0, 1, 2, DWD_Zero};
SDRM_BIG_NUM	_BN_One		= {0, 1, 2, DWD_One};

SDRM_BIG_NUM	*BN_Zero	= &_BN_Zero;
SDRM_BIG_NUM	*BN_One		= &_BN_One;


////////////////////////////////////////////////////////////////////////////
// Local Functon Protypes
////////////////////////////////////////////////////////////////////////////
int SDRM_DWD_Classical_REDC(cc_u32 *pdDest, cc_u32 DstLen, cc_u32 *pdModulus, cc_u32 ModLen);




#define SDRM_HL(A)	(cc_u32)(((A) >> 16) & 0xffffu)
#define SDRM_LL(A)	(cc_u32)((A) & 0xffffu)
#define SDRM_LH(A)	(cc_u32)(((A) & 0xffffu) << 16)
#define NOT(A)		(~(A))

#ifdef _OP64_NOTSUPPORTED
/*!	\brief	Double-width UINT32 Multiplication
 * \n	Dest		[out]destination, 2-cc_u32-size array
 * \n	Src1		[in]first element
 * \n	Src2		[in]second element
 * \return	void
 */
void SDRM_DIGIT_Mul(cc_u32 *Dest, cc_u32 Src1, cc_u32 Src2)
{
	cc_u32	Da, Db, R1, R0;

	R1 = SDRM_HL(Src1) * SDRM_HL(Src2);
	R0 = SDRM_LL(Src1) * SDRM_LL(Src2);

	Da = SDRM_HL(Src1) * SDRM_LL(Src2);
	Db = SDRM_LL(Src1) * SDRM_HL(Src2);

	if ((Db += Da) < Da)
		R1 += ((cc_u32)1) << 16;
	if ((R0 += SDRM_LH(Db)) < SDRM_LH(Db))
		R1++;

	Dest[0] = R0;
	Dest[1] = R1 + SDRM_HL(Db);
	
	return;
}

/*!	\brief	Doublue-width DWROD Division
 * \n	Src1		[in]upper-digit of dividend
 * \n	Src2		[in]lower-digit of dividend
 * \n	Div			[in]divisor
 */
cc_u32 SDRM_DIGIT_Div(cc_u32 Src1, cc_u32 Src2, cc_u32 Div)
{
	cc_u32	Dq, Dr, Dx, Dy, Dt;
	cc_u64 mod,temp, temp1;

	//	Estimate high half of quotient
	temp = Src1;
	mod = do_div(temp , (SDRM_HL(Div) + 1));
	Dq = (cc_u32)temp ;//do_div(Src1 , (SDRM_HL(Div) + 1));

	//	Subtract the product of estimate and divisor from the dividend
	Dr = Src1 - (SDRM_HL(Div) * Dq);
	Dx = SDRM_HL(Dr);
	Dy = SDRM_LH(Dr) + SDRM_HL(Src2);
	if ((Dr = Dy - (SDRM_LL(Div) * Dq)) > Dy)
		Dx--;

	//	Correct estimate
	while ((Dx > 0) || ((Dx == 0) && (Dr >= Div))) {
		if((Dr -= Div) > NOT(Div))
			Dx--;
		Dq++;
	}
	Dt = SDRM_LH(Dq);

	//	Estimate low half of quotient
	temp1 = Dr;
	mod = do_div(temp1,(SDRM_HL(Div) + 1));
	Dq = (cc_u32)temp1;

	//	Subtract the product of estimate and divisor from the dividend
	Dr -= SDRM_HL(Div) * Dq;
	Dx = SDRM_HL(Dr);
	Dy = SDRM_LH(Dr) + SDRM_LL(Src2);
	if ((Dr = Dy - (SDRM_LL(Div) * Dq)) > Dy)
		Dx--;

	//	Correct estimate
	while ((Dx > 0) || ((Dx == 0) && (Dr >= Div))) {
		if ((Dr -= Div) > NOT(Div))
			Dx--;
		Dq++;
	}

	return(Dt + Dq);
}

/*!	\brief	Doublue-width DWROD Modular
 * \n	Src1		[in]upper-digit of dividend
 * \n	Src2		[in]lower-digit of dividend
 * \n	Div			[in]divisor
 */
cc_u32 SDRM_DIGIT_Mod(cc_u32 Src1, cc_u32 Src2, cc_u32 Div)
{
	cc_u32	Dq, Dr, Dx, Dy;
	cc_u64 mod, temp, temp1;

	//	Estimate high half of quotient
	temp = Src1;
	mod = do_div(temp ,(SDRM_HL(Div) + 1)); 
	Dq = (cc_u32)temp;//do_div(Src1 ,(SDRM_HL(Div) + 1));

	//	Subtract the from dividend the product of estimate and divisor
	Dr = Src1 - (SDRM_HL(Div) * Dq);
	Dx = SDRM_HL(Dr);
	Dy = SDRM_LH(Dr) + SDRM_HL(Src2);
	if ((Dr = Dy - (SDRM_LL(Div) * Dq)) > Dy )
		Dx--;

	//	Correct estimate
	while ((Dx > 0) || ((Dx == 0) && (Dr >= Div)))
		if ((Dr -= Div) > NOT(Div))
			Dx--;

	//	Estimate low half of quotient
	temp1 = Dr;
	mod = do_div(temp1 ,(SDRM_HL(Div) + 1));
	Dq = (cc_u32)temp1;//do_div(Dr ,(SDRM_HL(Div) + 1));

	//	Subtract the from dividend the product of estimate and divisor
	Dr -= SDRM_HL(Div) * Dq;
	Dx = SDRM_HL(Dr);
	Dy = SDRM_LH(Dr) + SDRM_LL(Src2);
	if ((Dr = Dy - (SDRM_LL(Div) * Dq)) > Dy)
		Dx--;

	//	Correct estimate
	while ((Dx > 0) || ((Dx == 0) && (Dr >= Div)))
		if ((Dr -= Div) > NOT(Div) )
			Dx--;

	return( Dr );
}


#endif //_OP64_NOTSUPPORTED
#ifndef _OP64_NOTSUPPORTED
cc_u32 SDRM_DIGIT_Div(cc_u32 Src1, cc_u32 Src2, cc_u32 Div)
#if 1
{
	cc_u32	Dq, Dr, Dx, Dy, Dt;
	cc_u64 mod,temp, temp1;

	//	Estimate high half of quotient
	temp = Src1;
	mod = do_div(temp , (SDRM_HL(Div) + 1));
	Dq = (cc_u32)temp ;//do_div(Src1 , (SDRM_HL(Div) + 1));

	//	Subtract the product of estimate and divisor from the dividend
	Dr = Src1 - (SDRM_HL(Div) * Dq);
	Dx = SDRM_HL(Dr);
	Dy = SDRM_LH(Dr) + SDRM_HL(Src2);
	if ((Dr = Dy - (SDRM_LL(Div) * Dq)) > Dy)
		Dx--;

	//	Correct estimate
	while ((Dx > 0) || ((Dx == 0) && (Dr >= Div))) {
		if((Dr -= Div) > NOT(Div))
			Dx--;
		Dq++;
	}
	Dt = SDRM_LH(Dq);

	//	Estimate low half of quotient
	temp1 = Dr;
	mod = do_div(temp1,(SDRM_HL(Div) + 1));
	Dq = (cc_u32)temp1;

	//	Subtract the product of estimate and divisor from the dividend
	Dr -= SDRM_HL(Div) * Dq;
	Dx = SDRM_HL(Dr);
	Dy = SDRM_LH(Dr) + SDRM_LL(Src2);
	if ((Dr = Dy - (SDRM_LL(Div) * Dq)) > Dy)
		Dx--;

	//	Correct estimate
	while ((Dx > 0) || ((Dx == 0) && (Dr >= Div))) {
		if ((Dr -= Div) > NOT(Div))
			Dx--;
		Dq++;
	}

	return(Dt + Dq);
}
#else
{
	cc_u32	temp ;
	cc_u64 	mod;


	temp = (((cc_u64)(Src1) << SDRM_BitsInDWORD) ^ (Src2));
	mod = do_div(temp,Div);
	return temp;
}
#endif
#endif

/*!	\brief	cc_u32 Array Comparison
 * \param	pdSrc1		[in]first element
 * \param	dSrcLen1	[in]legnth of pdSrc1
 * \param	pdSrc2		[in]second element
 * \param	dSrcLen2	[in]legnth of pdSrc2
 * \return	1 if pdSrc1 is larger than pdSrc2
 * \n		0 if same
 * \n		-1 if pdSrc2 is larger than pdSrc1
 */
static int SDRM_DWD_Cmp(cc_u32 *pdSrc1, cc_u32 dSrcLen1, cc_u32 *pdSrc2, cc_u32 dSrcLen2)
{
	cc_u32	i;

	//When the length is different
	if (dSrcLen1 >= dSrcLen2) {
		for (i = dSrcLen1 - 1; i != dSrcLen2 - 1; i--) {
			if (pdSrc1[i]) {
				return +1;
			}
		}
	}
	else {
		for (i = dSrcLen2 - 1; i != dSrcLen1 - 1; i--) {
			if (pdSrc2[i]) {
				return -1;
			}
		}
	}

	//Compare common digits
	for (; i != (cc_u32)-1; i--) {
		if (pdSrc1[i] == pdSrc2[i]) {
			continue;
		}
		else  {
			if (pdSrc1[i] > pdSrc2[i]) {
				return +1;
			}
			else {
				return -1;
			}
		}
	}

	return 0;
}


/*!	\brief	Shift left the digit array
 * \param	pdDest		[out]destination
 * \param	pdSrc		[in]source
 * \param	dSrcLen		[in]legnth of pdSrc
 * \param	dNumOfShift	[in]shift amount
 * \return	carry
 */
static cc_u32 SDRM_DWD_SHL(cc_u32 *pdDest, cc_u32 *pdSrc, cc_u32 dSrcLen, cc_u32 dNumOfShift)
{
	cc_u32	i = dSrcLen - 1;
	cc_u32	dRet;

	dRet = pdSrc[i] >> (SDRM_BitsInDWORD - dNumOfShift);

	for (; i != 0; i--) {
		pdDest[i] = (pdSrc[i] << dNumOfShift) ^ (pdSrc[i - 1] >> (SDRM_BitsInDWORD - dNumOfShift));
	}

	pdDest[i] = pdSrc[i] << dNumOfShift;

	return dRet;
}


/*!	\brief	Shift right the digit array
 * \param	pdDest		[out]destination
 * \param	pdSrc		[in]source
 * \param	dSrcLen		[in]legnth of pdSrc
 * \param	dNumOfShift	[in]shift amount
 * \return	carry
 */
static cc_u32 SDRM_DWD_SHR(cc_u32 *pdDest, cc_u32 *pdSrc, cc_u32 dSrcLen, cc_u32 dNumOfShift)
{
	cc_u32 i = 0;
	cc_u32 dRet;

	dRet = pdSrc[i] << (SDRM_BitsInDWORD - dNumOfShift);

	for (; i < dSrcLen - 1; i++) {
		pdDest[i] = (pdSrc[i] >> dNumOfShift) ^ (pdSrc[i + 1] << (SDRM_BitsInDWORD - dNumOfShift));
	}

	pdDest[i] = pdSrc[i] >> dNumOfShift;

	return dRet;
}
 

/*!	\brief	Add two digit array
 * \param	pdDest		[out]destination
 * \param	pdSrc1		[in]first element
 * \param	dSrcLen1	[in]legnth of pdSrc1
 * \param	pdSrc2		[in]second element
 * \param	dSrcLen2	[in]legnth of pdSrc2
 * \return	carry
 */
static cc_u32 SDRM_DWD_Add(cc_u32 *pdDest, cc_u32 *pdSrc1, cc_u32 dSrcLen1, cc_u32 *pdSrc2, cc_u32 dSrcLen2)
{
	cc_u32	i;
	cc_u32	dCarry = 0, dTemp;

	//add low digits
	for (i = 0; i < dSrcLen2; i++) {
		if ((pdSrc2[i] == ((cc_u32)-1)) && (dCarry)) {
			pdDest[i] = pdSrc1[i];
		} else {
			dTemp = pdSrc2[i] + dCarry;
			pdDest[i] = pdSrc1[i] + dTemp;
			dCarry = (pdDest[i] < dTemp ) ? 1 : 0;
		}
	}

	//copy high digits
	if (dSrcLen1 > i)
		memcpy(pdDest + i, pdSrc1 + i, (dSrcLen1 - i) * SDRM_SIZE_OF_DWORD);

	//process carry
	if (!dCarry) {
		return 0;
	} else {
		for (i = dSrcLen2; i < dSrcLen1; i++) {
			if (++pdDest[i]) {
				return 0;
			}
		}
	}

	return 1;
}


/*!	\brief	subtract digit array
 * \param	pdDest		[out]destination
 * \param	pdSrc1		[in]first element
 * \param	dSrcLen1	[in]legnth of pdSrc1
 * \param	pdSrc2		[in]second element
 * \param	dSrcLen2	[in]legnth of pdSrc2
 * \return	carry
 */
static cc_u32 SDRM_DWD_Sub(cc_u32 *pdDest, cc_u32 *pdSrc1, cc_u32 dSrcLen1, cc_u32 *pdSrc2, cc_u32 dSrcLen2)
{
	cc_u32	i;
	cc_u32	dCarry = 0, dTemp;

	//subtract low digits
	for (i = 0; i < dSrcLen2; i++) {
		if (pdSrc2[i] + dCarry == 0) {
			pdDest[i] = pdSrc1[i];
		} else {
			dTemp = pdSrc2[i] + dCarry;
			pdDest[i] = pdSrc1[i] - dTemp;
			dCarry = ((pdDest[i]) > ~(dTemp)) ? 1 : 0;
		}
	}

	//copy high digits
	if (dSrcLen1 > i)
		memcpy(pdDest + i, pdSrc1 + i, (dSrcLen1 - i) * SDRM_SIZE_OF_DWORD);
 
	//process carry
	if (!dCarry) {
		return 0;
	} else {
		for (i = dSrcLen2  ; i < dSrcLen1; i++)
			if (pdDest[i]--) {
				return 0;
			}
		}	

	return (~0);
}


/*!	\brief	Add multiple
 * \param	pdDest		[out]destination
 * \param	dDstLen		[in]legnth of pbDest
 * \param	pdSrc		[in]source
 * \param	dSrcLen		[in]legnth of pdSrc
 * \param	dMultiplier	[in]multiplier
 * \return	carry
 */
static cc_u32 SDRM_DWD_MulAdd(cc_u32 *pdDest, cc_u32 dDstLen, cc_u32 *pdSrc, cc_u32 dSrcLen, cc_u32 dMultiplier)
{
	cc_u32 i;
	cc_u32 pdDigit[2], dTemp = 0;

	//Multiplication part
	for (i = 0; i < dSrcLen; i++) {
		SDRM_DIGIT_Mul(pdDigit, dMultiplier, pdSrc[i]);
		if ((dTemp += pdDigit[0]) < pdDigit[0]) {
			pdDigit[1]++;
		}

		if ((pdDest[i] += dTemp) < dTemp) {
			pdDigit[1]++;
		}

		dTemp = pdDigit[1];
	}

	if (i == dDstLen) {
		return dTemp;
	}

	//process top digit
	if ((pdDest[i] += dTemp) >= dTemp) {
		return 0;
	}

	for (i++; i < dDstLen; i++) {
		if ((++pdDest[i]) != 0) {
			return 0;
		}
	}

	return 1;
}

/*!	\brief	Multiply and Subtract Digit Array
 * \param	pdDest		[out]destination
 * \param	dDstLen		[in]legnth of pbDest
 * \param	pdSrc		[in]source
 * \pggaram	dSrcLen		[in]legnth of pdSrc
 * \param	dMultiplier	[in]multiplier
 * \return	carry
 */
static cc_u32 SDRM_DWD_MulSub(cc_u32 *pdDest, cc_u32 dDstLen, cc_u32 *pdSrc, cc_u32 dSrcLen, cc_u32 dMultiplier)
{
	cc_u32	i;
	cc_u32	pdDigit[2], dTemp = 0;

	//Multiplication part
	for (i = 0; i < dSrcLen; i++) {
		SDRM_DIGIT_Mul(pdDigit, dMultiplier, pdSrc[i]);
		dTemp += pdDigit[0];

		if (dTemp < pdDigit[0]) {
			pdDigit[1]++;
		}

		if (pdDest[i] < dTemp) {
			pdDigit[1]++;
		}

		pdDest[i] -= dTemp;
		dTemp = pdDigit[1];
	}

	if (i == dDstLen) {
		return dTemp;
	}

	//process top digit
	if (pdDest[i] >= dTemp) {
		pdDest[i] -= dTemp;

		return 0;
	} else {
		pdDest[i] -= dTemp;
	}

	for (i++; i < dDstLen; i++)
		if ((pdDest[i]--) != 0) {
			return 0;
		}

	return 1;
}


/*!	\brief	Multiply tow Digit array
 * \param	pdDest		[out]destination
 * \param	pdSrc1		[in]first element
 * \param	dSrcLen1	[in]legnth of pdSrc1
 * \param	pdSrc2		[in]second element
 * \param	dSrcLen2	[in]legnth of pdSrc2
 * \return	void
 */
static void SDRM_DWD_Mul(cc_u32 *pdDest, cc_u32 *pdSrc1, cc_u32 dSrcLen1, cc_u32 *pdSrc2, cc_u32 dSrcLen2)
{
	cc_u32	i, j;
	cc_u32	pdDigit[2], dTemp;

	memset(pdDest, 0, (dSrcLen1 + dSrcLen2) * SDRM_SIZE_OF_DWORD);

	for (j = 0; j < dSrcLen2; j++) {
		dTemp = 0;
		for (i = 0; i < dSrcLen1; i++) {
			SDRM_DIGIT_Mul(pdDigit, pdSrc1[i], pdSrc2[j]);
			if ((dTemp += pdDigit[0]) < pdDigit[0]) {
				pdDigit[1]++;
			}

			if ((pdDest[i + j] += dTemp) < dTemp) {
				pdDigit[1]++;
			}

			dTemp = pdDigit[1];
		}
		pdDest[i + j] = dTemp;
	}
}


/*!	\brief	Multiply tow Digit array
 * \param	pdDest		[out]quotient
 * \param	pdRem		[out]remainder
 * \param	pdSrc1		[in]divisor
 * \param	dSrcLen1	[in]legnth of pdSrc1
 * \param	pdSrc2		[in]dividend
 * \param	dSrcLen2	[in]legnth of pdSrc2
 * \return	0 if reaminder is zero
 * \n		1 otherwise
 */
static cc_u32 SDRM_DWD_Div(cc_u32 *pdDest, cc_u32 *pdRem,
						   cc_u32 *pdSrc1, cc_u32 dSrcLen1,
						   cc_u32 *pdSrc2, cc_u32 dSrcLen2)
{
	cc_u32	i, q, c, dNum_of_Shift = 0;
	cc_u32	*C = (cc_u32*)kmalloc(SDRM_SIZE_OF_DWORD * (2 * (MAX2(dSrcLen1, dSrcLen2)) + 2),GFP_KERNEL);

	if (!C) {
		return (cc_u32)CRYPTO_MEMORY_ALLOC_FAIL;
	}

	SDRM_DWD_Copy(C, pdSrc1, dSrcLen1);
	C[dSrcLen1] = 0;
	c = dSrcLen1 + 1;

	//Remove lowest '0's
	for (i = dSrcLen2 * SDRM_BitsInDWORD-1; !SDRM_CheckBitUINT32(pdSrc2, i); i--, dNum_of_Shift++);
		 
	if (dNum_of_Shift) {
		SDRM_DWD_SHL(C, C, c, dNum_of_Shift);
		SDRM_DWD_SHL(pdSrc2, pdSrc2, dSrcLen2, dNum_of_Shift);
	}

	for (i = c-dSrcLen2 - 1; i != (cc_u32)-1; i--) {
		if (C[dSrcLen2 + i]==pdSrc2[dSrcLen2 - 1] ) {
			q = (cc_u32)-1;
		}
		else {
			q = SDRM_DIGIT_Div(C[dSrcLen2 + i], C[dSrcLen2 + i - 1], pdSrc2[dSrcLen2 - 1]);
		}

		if (SDRM_DWD_MulSub(C + i, dSrcLen2 + 1, pdSrc2, dSrcLen2, q) ) {
			q--;
			if (!SDRM_DWD_Add(C + i, C + i, dSrcLen2 + 1, pdSrc2, dSrcLen2)) {
				q--;
				SDRM_DWD_Add(C + i, C + i, dSrcLen2 + 1, pdSrc2, dSrcLen2);
			}
		}
		pdDest[i] = q;
	}

	//Recover lowest '0's
	if (dNum_of_Shift) {
		SDRM_DWD_SHR(pdSrc2, pdSrc2, dSrcLen2, dNum_of_Shift);
		SDRM_DWD_SHR(C, C, dSrcLen2, dNum_of_Shift);
	}

	if (pdRem) {
		SDRM_DWD_Copy(pdRem, C, dSrcLen2);
	}

	for (i = 0; i < c; i++) {
		if (C[i]) {
			kfree(C);

			return 1;
		}
	}
	kfree(C);

	return 0;
}


/*!	\brief	Classical Modular Reduction Algorithm
 * \param	pdDest		[out]destination
 * \param	DstLen		[in]length of pdDest
 * \param	pdModulus	[in]modulus
 * \param	ModLen		[in]legnth of pdModulus
 * \return	CRYPTO_SUCCESS	if no error is occured
 */
int SDRM_DWD_Classical_REDC(cc_u32 *pdDest, cc_u32 DstLen, cc_u32 *pdModulus, cc_u32 ModLen)
{
	cc_u32	i;
	cc_u32	MSB=0, TTTT=0, FLAG=0, D_Quotient, MSD_Modulus;

	if (DstLen < ModLen) {
		return CRYPTO_SUCCESS;
	}

	if (pdDest[DstLen - 1] >= pdModulus[ModLen - 1]) {
		FLAG++;
		TTTT = pdDest[DstLen];
		pdDest[DstLen++] = 0;
	}

	for (i = SDRM_BitsInDWORD - 1; i != (cc_u32)-1; i--) {
		if (pdModulus[ModLen - 1] & ((cc_u32)1 << i)) {
			break;
		}

		MSB++;
	}

	if (MSB) {
		SDRM_DWD_SHL(pdModulus, pdModulus, ModLen, MSB);
		SDRM_DWD_SHL(pdDest, pdDest, DstLen, MSB);
	}

	//	Step 2 : main part
	MSD_Modulus = pdModulus[ModLen - 1];
	for (i = DstLen - ModLen - 1; i != (cc_u32)-1; i--) {
		//	Step 2-1 : Estimate D_Quotient
		if (pdDest[ModLen + i] == MSD_Modulus) {
			D_Quotient = (cc_u32)-1;
		}
		else {
			D_Quotient = SDRM_DIGIT_Div(pdDest[ModLen + i], pdDest[ModLen + i - 1], MSD_Modulus);
		}

		//	Step 2-2 : Make pdDest <- pdDest-D_Quotient*pdModulus
		if (SDRM_DWD_MulSub(pdDest + i, ModLen + 1, pdModulus, ModLen, D_Quotient) ) {
			if (SDRM_DWD_Add(pdDest + i, pdDest + i, ModLen + 1, pdModulus, ModLen) == 0) {
				SDRM_DWD_Add(pdDest + i, pdDest + i, ModLen + 1, pdModulus, ModLen);
			}
		}
	}

	//	Step 4 : inverse part of Step 2
	if (MSB) {
		SDRM_DWD_SHR(pdModulus, pdModulus, ModLen, MSB);
		SDRM_DWD_SHR(pdDest, pdDest, ModLen, MSB);
	}

	//	Step 4.5 : inverse part of Step 1.5
	if (FLAG) {
		DstLen--;
		pdDest[DstLen] = TTTT;
	}

	return CRYPTO_SUCCESS;
}


/*!	\brief	Convert Octet String to Big Number
 * \param	pbSrc	[in]source octet string
 * \param	dSrcLen	[in]Byte-length of pbSrc
 * \param	BN_Dst	[out]output big number
 * \return	CRYPTO_SUCCESS	if no error is occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if arrary is too small
 */
int	SDRM_OS2BN(cc_u8 *pbSrc, cc_u32 dSrcLen, SDRM_BIG_NUM *BN_Dst)
{
	cc_u32	i;
	int		ret;
	cc_u64  mod, temp;

	if (!BN_Dst) {
		temp = (dSrcLen - 1);
		mod = do_div(temp, 2);
		BN_Dst = SDRM_BN_Init((cc_u32)temp + 1);
	}

	SDRM_BN_Clr(BN_Dst);

	for (i = 0; i < dSrcLen; i++) {
		ret = SDRM_BN_SHL(BN_Dst, BN_Dst, 8);

		if (ret != CRYPTO_SUCCESS) {
			return ret;
		}

		BN_Dst->pData[0] ^= pbSrc[i];
		if (BN_Dst->Length == 0)
			BN_Dst->Length = 1;
	}
	return CRYPTO_SUCCESS;
}



/*!	\brief	Clear the SDRM_BIG_NUM structure
 * \param	BN_Src		[in]source
 * \return	CRYPTO_SUCCESS
 */
int SDRM_BN_Clr(SDRM_BIG_NUM* BN_Src) {
	BN_Src->sign = 0;
	BN_Src->Length = 0;
	memset(BN_Src->pData, 0, BN_Src->Size * SDRM_SIZE_OF_DWORD);

	return CRYPTO_SUCCESS;
}


/*!	\brief	copy SDRM_BIG_NUM
 * \param	BN_Dest		[out]destination
 * \param	BN_Src		[in]source
 * \return	CRYPTO_SUCCESS
 */
int SDRM_BN_Copy(SDRM_BIG_NUM* BN_Dest, SDRM_BIG_NUM* BN_Src) {
	if (BN_Src->Length > BN_Dest->Size) {
		return CRYPTO_BUFFER_TOO_SMALL;
	}

	BN_Dest->sign = BN_Src->sign;
	BN_Dest->Length = BN_Src->Length;
	memcpy(BN_Dest->pData, BN_Src->pData, BN_Src->Length * SDRM_SIZE_OF_DWORD);

	return CRYPTO_SUCCESS;
}


/*!	\brief	allocate big number from buffer
 * \param	pbSrc		[in]start pointer of buffer
 * \param	dSize		[in]buffer size of big number
 * \return	pointer of SDRM_BIG_NUM structure
 */
SDRM_BIG_NUM *SDRM_BN_Alloc(cc_u8* pbSrc, cc_u32 dSize) {
	SDRM_BIG_NUM	*BN_Dest = (SDRM_BIG_NUM*)(void*)pbSrc;

	memset(BN_Dest, 0, sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD);
	BN_Dest->pData = (cc_u32*)(void*)(pbSrc + sizeof(SDRM_BIG_NUM));
	BN_Dest->Size = dSize;

	return BN_Dest;
}


/*!	\brief	Allocate a new big number object
 * \param	dSize		[in]buffer size of big number
 * \return	pointer of SDRM_BIG_NUM structure
 * \n		NULL if memory allocation is failed
 */
SDRM_BIG_NUM *SDRM_BN_Init(cc_u32 dSize) {
	cc_u32			AllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;
	cc_u8			*pbBuf = (cc_u8*)kmalloc(AllocSize,GFP_KERNEL);
	SDRM_BIG_NUM	*BN_Src = (SDRM_BIG_NUM*)(void*)pbBuf;
	if (BN_Src == NULL) {
		return NULL;
	}

	memset(BN_Src, 0, AllocSize);
	BN_Src->pData = (cc_u32*)(void*)(pbBuf + sizeof(SDRM_BIG_NUM));
	BN_Src->Size = dSize;

	return BN_Src;
}


/*!	\brief	Compare two Big Number
 * \param	BN_Src1		[in]first element
 * \param	BN_Src2		[in]second element
 * \return	1 if BN_Src1 is larger than pdSrc2
 * \n		0 if same
 * \n		-1 if BN_Src2 is larger than pdSrc1
 */
int SDRM_BN_Cmp(SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2)
{
	if (BN_Src1->Length >= BN_Src2->Length) {
		return  SDRM_DWD_Cmp(BN_Src1->pData, BN_Src1->Length,
							 BN_Src2->pData, BN_Src2->Length);
	}
	else {
		return -SDRM_DWD_Cmp(BN_Src2->pData, BN_Src2->Length,
							 BN_Src1->pData, BN_Src1->Length);
	}
}





/*!	\brief	Big Number Shift Left
 * \param	BN_Dst		[out]destination
 * \param	BN_Src		[in]source
 * \param	NumOfShift	[in]shift amount
 * \return	CRYPTO_SUCCESS if no error occured
 */
int SDRM_BN_SHL(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, cc_u32 NumOfShift)
{
	cc_u32	t;
	cc_u64 mod = 0,temp = 0;

	if (!BN_Src->Length) {
		SDRM_BN_Copy(BN_Dst, BN_Zero);
		return CRYPTO_SUCCESS;
	}

	BN_Dst->sign = BN_Src->sign;

	t = NumOfShift % SDRM_BitsInDWORD;
    
	if (t) {
		BN_Dst->Length = BN_Src->Length;
		t = SDRM_DWD_SHL(BN_Dst->pData, BN_Src->pData, BN_Src->Length, t);
        
		if (t) {
			BN_Dst->pData[BN_Dst->Length++] = t;
		}
	}
	temp = (cc_u64) NumOfShift;
    
	mod = do_div(temp,SDRM_BitsInDWORD);

	t = temp;//do_div(NumOfShift,SDRM_BitsInDWORD);
	if (t) {
		BN_Dst->Length = BN_Src->Length + t;

		memcpy((BN_Dst->pData) + t, (BN_Src->pData), (BN_Dst->Length - t) * SDRM_SIZE_OF_DWORD);

		memset(BN_Dst->pData, 0, t * SDRM_SIZE_OF_DWORD);
	}

	SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);

	return CRYPTO_SUCCESS;
}


/*!	\brief	Big Number Shift Right
 * \param	BN_Dst		[out]destination
 * \param	BN_Src		[in]source
 * \param	NumOfShift	[in]shift amount
 * \return	CRYPTO_SUCCESS if no error occured
 */
int SDRM_BN_SHR(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, cc_u32 NumOfShift)
{
	cc_u32	t;
	cc_u64 mod,temp;

	if (!BN_Src->Length) {
		SDRM_BN_Copy(BN_Dst, BN_Src);
		return CRYPTO_SUCCESS;
	}
	temp = NumOfShift;
	mod = do_div(temp, SDRM_BitsInDWORD);
	t = temp;//do_div(NumOfShift, SDRM_BitsInDWORD);
	if (t) {
		if (t >= BN_Src->Length) {
			SDRM_BN_Copy(BN_Dst, BN_Zero);
			return CRYPTO_SUCCESS;
		}

		memcpy(BN_Dst->pData, (BN_Src->pData) + t, (BN_Src->Length - t) * SDRM_SIZE_OF_DWORD);

		BN_Dst->Length = BN_Src->Length - t;
		BN_Dst->sign = BN_Src->sign;
		SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);
	}
	else {
		SDRM_BN_Copy(BN_Dst, BN_Src);
	}

	t = NumOfShift % SDRM_BitsInDWORD;
	if (t) {
		SDRM_DWD_SHR(BN_Dst->pData, BN_Dst->pData, BN_Dst->Length, t);
	}

	SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);

	return CRYPTO_SUCCESS;
}


/*!	\brief	Big Number Addition
 * \param	BN_Dst		[out]destination
 * \param	BN_Src1		[in]first element
 * \param	BN_Src2		[in]second element
 * \return	CRYPTO_SUCCESS				if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Add(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2)
{
	cc_u32			carry, dSize, dAllocSize;
	SDRM_BIG_NUM	*temp, *temp_Src1, *temp_Src2;
	cc_u8			*pbBuf;
	
	dSize = MAX2(BN_Src1->Size, BN_Src2->Size);
	dAllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;

	pbBuf = (cc_u8*)kmalloc((dAllocSize * 2),GFP_KERNEL);
	
	if (!pbBuf) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	temp_Src1 = SDRM_BN_Alloc(pbBuf, dSize);
	temp_Src2 = SDRM_BN_Alloc(pbBuf + dAllocSize, dSize);

	if (!BN_Src1->Length) {
		SDRM_BN_Copy(BN_Dst, BN_Src2);
		kfree(pbBuf);
		return CRYPTO_SUCCESS;
	}

	if (!BN_Src2->Length) {
		SDRM_BN_Copy(BN_Dst, BN_Src1);
		kfree(pbBuf);
		return CRYPTO_SUCCESS;
	}

	SDRM_BN_Copy(temp_Src1, BN_Src1);
	SDRM_BN_Copy(temp_Src2, BN_Src2);

	if (temp_Src1->sign ^ temp_Src2->sign) {
		if (temp_Src1->sign){
			temp = temp_Src1;
			temp_Src1 = temp_Src2;
			temp_Src2 = temp;
		}

		if (SDRM_BN_Cmp(temp_Src1, temp_Src2) < 0) {
			SDRM_DWD_Sub(BN_Dst->pData, temp_Src2->pData, temp_Src2->Length, temp_Src1->pData, temp_Src1->Length);
			BN_Dst->sign = 1;
			BN_Dst->Length = temp_Src2->Length;
		}
		else{
			SDRM_DWD_Sub(BN_Dst->pData, temp_Src1->pData, temp_Src1->Length, temp_Src2->pData, temp_Src2->Length);
			BN_Dst->sign = 0;
			BN_Dst->Length = temp_Src1->Length;
		}
		kfree(pbBuf);

		return CRYPTO_SUCCESS;
	}

	if (temp_Src1->sign)
		BN_Dst->sign = 1;
	else
		BN_Dst->sign = 0;

	if (temp_Src1->Length > temp_Src2->Length) {
		BN_Dst->Length = temp_Src1->Length;
		carry = SDRM_DWD_Add(BN_Dst->pData, temp_Src1->pData, temp_Src1->Length, temp_Src2->pData, temp_Src2->Length);
		if (carry) {
			BN_Dst->pData[BN_Dst->Length++] = carry;
		}

	}
	else {
		BN_Dst->Length = temp_Src2->Length;
		carry = SDRM_DWD_Add(BN_Dst->pData, temp_Src2->pData, temp_Src2->Length, temp_Src1->pData, temp_Src1->Length);
		if ( carry ) {
			BN_Dst->pData[BN_Dst->Length++] = carry;
		}
	}
	
	SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);

	kfree(pbBuf);

	return CRYPTO_SUCCESS;
}


/*!	\brief	Big Number Subtraction
 * \param	BN_Dst		[out]destination
 * \param	BN_Src1		[in]first element
 * \param	BN_Src2		[in]second element
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Sub(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2)
{
	int				i, add = 0, neg = 0, dSize, dAllocSize; 
	SDRM_BIG_NUM	*temp, *temp_Src1, *temp_Src2;
	cc_u8			*pbBuf;
		
	dSize = MAX2(BN_Src1->Size, BN_Src2->Size);
	dAllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;
	pbBuf = (cc_u8*)kmalloc((dAllocSize * 2),GFP_KERNEL);
	
	if (!pbBuf) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	temp_Src1 = SDRM_BN_Alloc(pbBuf, dSize);
	temp_Src2 = SDRM_BN_Alloc(pbBuf + dAllocSize, dSize);

	SDRM_BN_Copy(temp_Src1, BN_Src1);
	SDRM_BN_Copy(temp_Src2, BN_Src2);

	if (BN_Src1 == BN_Src2){
		SDRM_BN_Clr(BN_Dst);
		kfree(pbBuf);

		return CRYPTO_SUCCESS;
	}

	//to process sign
	if (temp_Src1->sign){
		if (temp_Src2->sign){
			temp = temp_Src1;
			temp_Src1 = temp_Src2;
			temp_Src2 = temp;
		}
		else{
			add = 1;
			neg = 1;
		}
	}
	else{
		if (temp_Src2->sign){
			add = 1;
			neg = 0;
		}
	}

	if (add){
		i = (temp_Src1->Length | temp_Src2->Length) +1;
		if (i) {
			SDRM_BN_Add(BN_Dst, temp_Src1, temp_Src2);
		}
		else {
			SDRM_BN_Add(BN_Dst, temp_Src2, temp_Src1);
		}

		BN_Dst->sign = neg;
		kfree(pbBuf);

		return CRYPTO_SUCCESS;
	}

	if (SDRM_BN_Cmp(temp_Src1, temp_Src2) < 0){
		SDRM_DWD_Sub(BN_Dst->pData, temp_Src2->pData, temp_Src2->Length, temp_Src1->pData, temp_Src1->Length);
		BN_Dst->sign = 1;
		BN_Dst->Length = temp_Src2->Length;
	}
	else{
		SDRM_DWD_Sub(BN_Dst->pData, temp_Src1->pData, temp_Src1->Length, temp_Src2->pData, temp_Src2->Length);
		BN_Dst->sign = 0;
		BN_Dst->Length = temp_Src1->Length;
	}
	SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);

	kfree(pbBuf);

	return CRYPTO_SUCCESS;
}


/*!	\brief	Big Number Multiplication
 * \param	BN_Dst			[out]destination
 * \param	BN_Multiplicand	[in]first element
 * \param	BN_Multiplier	[in]second element
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Mul(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Multiplicand, SDRM_BIG_NUM *BN_Multiplier)
{
	SDRM_BIG_NUM	*Dst = BN_Dst;
	cc_u8			SWAP_FLAG = 0;

	if ((BN_Dst == BN_Multiplicand) || (BN_Dst == BN_Multiplier)) {
		SWAP_FLAG = 1;
		BN_Dst = SDRM_BN_Init(BN_Dst->Size * 2);
		if (BN_Dst == NULL) {
			return CRYPTO_MEMORY_ALLOC_FAIL;
		}
	}

	BN_Dst->Length = BN_Multiplicand->Length + BN_Multiplier->Length;

	if (BN_Multiplicand->sign != BN_Multiplier->sign) BN_Dst->sign = 1;
	else BN_Dst->sign = 0;

	if ((BN_Multiplicand->Length == 0) || (BN_Multiplier->Length == 0)) {
		Dst->Length = 0;
		if (SWAP_FLAG == 1) {
			SDRM_BN_FREE(BN_Dst);
		}
		return CRYPTO_SUCCESS;
	}
	else if (BN_Multiplicand->Length > BN_Multiplier->Length) {
		SDRM_DWD_Mul(BN_Dst->pData,
				BN_Multiplicand->pData, BN_Multiplicand->Length,
				BN_Multiplier->pData, BN_Multiplier->Length);
	}
	else {
		SDRM_DWD_Mul(BN_Dst->pData,
				BN_Multiplier->pData, BN_Multiplier->Length,
				BN_Multiplicand->pData, BN_Multiplicand->Length);
	}

	SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);

	if (SWAP_FLAG) {
		SDRM_BN_Copy(Dst, BN_Dst);
		SDRM_BN_FREE(BN_Dst);
	}

	return CRYPTO_SUCCESS;
}


/*!	\brief	Big Number Division
 * \param	BN_Quotient		[out]quotient
 * \param	BN_Remainder	[out]remainder
 * \param	BN_Dividend		[in]dividend
 * \param	BN_Divisor		[in]divisor
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_Div(SDRM_BIG_NUM *BN_Quotient, SDRM_BIG_NUM *BN_Remainder,
				SDRM_BIG_NUM *BN_Dividend, SDRM_BIG_NUM *BN_Divisor)
{
	cc_u32			tmp, dSize, dAllocSize;
	SDRM_BIG_NUM	*temp_Dividend, *temp_Divisor;
	cc_u32			*bnTmp;
	cc_u8			*pbBuf;

	if(BN_Quotient == NULL ||(!BN_Quotient))
	{
		dSize = BN_Dividend->Size;
	}
	else
	{
		dSize = MAX2(BN_Quotient->Size, BN_Dividend->Size);
	}

	dAllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;
	pbBuf = (cc_u8*)kmalloc((dAllocSize * 3 + 2 * SDRM_SIZE_OF_DWORD),GFP_KERNEL);
	if (!pbBuf) return CRYPTO_MEMORY_ALLOC_FAIL;
	
	temp_Dividend	= SDRM_BN_Alloc(pbBuf, dSize);
	temp_Divisor	= SDRM_BN_Alloc(pbBuf + dAllocSize, dSize);
	bnTmp = (cc_u32*)(void*)(pbBuf + dSize + dAllocSize);

	SDRM_BN_Copy(temp_Dividend, BN_Dividend);
	SDRM_BN_Copy(temp_Divisor, BN_Divisor);

	if (SDRM_BN_Cmp(temp_Dividend, temp_Divisor) < 0) {
		if (BN_Remainder != NULL) {
			SDRM_BN_Copy(BN_Remainder, temp_Dividend);
			kfree(pbBuf);

			return CRYPTO_SUCCESS;
		}
		if (!BN_Quotient) BN_Quotient = SDRM_BN_Init(dSize);
		kfree(pbBuf);
		return CRYPTO_SUCCESS;
	}

	if (BN_Quotient == NULL) {
		BN_Remainder->Length = temp_Divisor->Length;

		tmp = SDRM_DWD_Div(bnTmp, BN_Remainder->pData,
					 temp_Dividend->pData, temp_Dividend->Length,
					 temp_Divisor->pData, temp_Divisor->Length);
		SDRM_BN_OPTIMIZE_LENGTH(BN_Remainder);
		BN_Remainder->sign = BN_Dividend->sign;
	} else if ( BN_Remainder==NULL ) {
		BN_Quotient->Length = temp_Dividend->Length - temp_Divisor->Length + 1;

		tmp = SDRM_DWD_Div(BN_Quotient->pData, bnTmp,
					 temp_Dividend->pData, temp_Dividend->Length,
					 temp_Divisor->pData, temp_Divisor->Length);
		SDRM_BN_OPTIMIZE_LENGTH(BN_Quotient);
		BN_Quotient->sign= (BN_Dividend->sign^BN_Divisor->sign);
	} else {
		BN_Quotient->Length = temp_Dividend->Length - temp_Divisor->Length + 1;
		BN_Remainder->Length = temp_Divisor->Length;
		BN_Quotient->sign= (BN_Dividend->sign^BN_Divisor->sign);
		BN_Remainder->sign = BN_Dividend->sign;

		tmp = SDRM_DWD_Div(BN_Quotient->pData, BN_Remainder->pData,
					 BN_Dividend->pData, BN_Dividend->Length,
					 BN_Divisor->pData, BN_Divisor->Length);

		SDRM_BN_OPTIMIZE_LENGTH(BN_Quotient);
		SDRM_BN_OPTIMIZE_LENGTH(BN_Remainder);
	}

	kfree(pbBuf);

	return CRYPTO_SUCCESS;
}





/*!	\brief	Big Number Modular Reduction
 * \param	BN_Dst		[out]destination
 * \param	BN_Src		[in]source
 * \param	BN_Modulus	[in]modular m
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 */
int SDRM_BN_ModRed(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src, SDRM_BIG_NUM *BN_Modulus)
{
	int			ret;
	cc_u32		*Value = (cc_u32*)kmalloc((SDRM_SIZE_OF_DWORD * (2 * (sizeof(SDRM_BIG_NUM)) + MAX2(BN_Src->Size, BN_Modulus->Size) + 2)),GFP_KERNEL);

	if (!Value) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	if (SDRM_BN_Cmp(BN_Src, BN_Modulus) < 0) {
		SDRM_BN_Copy(BN_Dst, BN_Src);
		kfree(Value);
		return CRYPTO_SUCCESS;
	}

	memcpy(Value, BN_Src->pData, BN_Src->Length * SDRM_SIZE_OF_DWORD);

	ret = SDRM_DWD_Classical_REDC(Value, BN_Src->Length,	BN_Modulus->pData, BN_Modulus->Length);

	if (ret != CRYPTO_SUCCESS) {
		kfree(Value);
		return ret;
	}

	memcpy(BN_Dst->pData, Value, BN_Modulus->Length * SDRM_SIZE_OF_DWORD);

	BN_Dst->Length = BN_Modulus->Length;
	BN_Dst->sign = BN_Modulus->sign;
	SDRM_BN_OPTIMIZE_LENGTH(BN_Dst);

	kfree(Value);

	return CRYPTO_SUCCESS;
}



/*!	\brief	Big Number Modular Inverse
 * \param	BN_Dest		[out]destination
 * \param	BN_Src		[in]soure
 * \param	BN_Modulus	[in]modular m
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 * \n		CRYPTO_NEGATIVE_INPUT		if source is negative value
 * \n		CRYPTO_INVERSE_NOT_EXIST	if inverse is not exists
 */
int SDRM_BN_ModInv(SDRM_BIG_NUM *BN_Dest, SDRM_BIG_NUM *BN_Src, SDRM_BIG_NUM *BN_Modulus) {
	SDRM_BIG_NUM	*BN_G0, *BN_G1, *BN_V0, *BN_V1, *BN_Y, *BN_Temp1, *BN_Temp2;
	cc_u8			*pbBuf = NULL;
	cc_u32			dSize, dAllocSize;

	dSize = MAX2(BN_Src->Size, BN_Modulus->Size);
	dAllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;

	pbBuf = (cc_u8*)kmalloc((dAllocSize * 7),GFP_KERNEL);

	if (!pbBuf) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	BN_G0 =    SDRM_BN_Alloc(pbBuf, dSize);
	BN_G1 =    SDRM_BN_Alloc((cc_u8*)BN_G0    + dAllocSize, dSize);
	BN_V0 =	   SDRM_BN_Alloc((cc_u8*)BN_G1    + dAllocSize, dSize);
	BN_V1 =	   SDRM_BN_Alloc((cc_u8*)BN_V0    + dAllocSize, dSize);
	BN_Y =     SDRM_BN_Alloc((cc_u8*)BN_V1    + dAllocSize, dSize);
	BN_Temp1 = SDRM_BN_Alloc((cc_u8*)BN_Y     + dAllocSize, dSize);
	BN_Temp2 = SDRM_BN_Alloc((cc_u8*)BN_Temp1 + dAllocSize, dSize);

	if (BN_Src->sign) {
		kfree(pbBuf);
		return CRYPTO_NEGATIVE_INPUT;
	}

	//Extended Euclid Algorithm
	SDRM_BN_Copy(BN_G0, BN_Modulus);
	SDRM_BN_ModRed(BN_G1, BN_Src, BN_Modulus);

	SDRM_BN_Copy(BN_V0, BN_Zero);
	SDRM_BN_Copy(BN_V1, BN_One);

	SDRM_BN_Clr(BN_Y);
	SDRM_BN_Clr(BN_Dest);

	while(SDRM_BN_Cmp(BN_G1, BN_Zero)) {
		if (!SDRM_BN_Cmp(BN_G1, BN_One)) {
			SDRM_BN_Copy(BN_Dest, BN_V1);
			SDRM_BN_OPTIMIZE_LENGTH(BN_Dest);
			kfree(pbBuf);

			return CRYPTO_SUCCESS;
		}

		SDRM_BN_Clr(BN_Y);
		SDRM_BN_Clr(BN_Temp1);
		SDRM_DWD_Div(BN_Y->pData, BN_Temp1->pData, BN_G0->pData, BN_G0->Length, BN_G1->pData, BN_G1->Length);

		BN_Y->Length = BN_G0->Length;
		SDRM_BN_OPTIMIZE_LENGTH(BN_Y);

		BN_Temp1->Length = BN_G1->Length;
		SDRM_BN_Copy(BN_G0, BN_Temp1);
		SDRM_BN_OPTIMIZE_LENGTH(BN_G0);

		SDRM_BN_Clr(BN_Temp1);
		SDRM_DWD_Mul(BN_Temp1->pData, BN_Y->pData, BN_Y->Length, BN_V1->pData, BN_V1->Length);
		BN_Temp1->Length = BN_Y->Length + BN_V1->Length;
		SDRM_BN_OPTIMIZE_LENGTH(BN_Temp1);

		SDRM_BN_Clr(BN_Temp2);
		if (SDRM_BN_Cmp(BN_V0, BN_Temp1) >= 0) {
			SDRM_BN_Add(BN_Temp2, BN_V0, BN_Temp1);
		}
		else {
			SDRM_BN_Add(BN_Temp2, BN_Temp1, BN_V0);
		}

		SDRM_BN_Copy(BN_V0, BN_Temp2);


		if (!SDRM_BN_Cmp(BN_G0, BN_Zero)) {
			break;
		}

		if (!SDRM_BN_Cmp(BN_G0, BN_One)) {
			SDRM_BN_Sub(BN_Dest, BN_Modulus, BN_V0);
			SDRM_BN_OPTIMIZE_LENGTH(BN_Dest);
			kfree(pbBuf);

			return CRYPTO_SUCCESS;
		}

		SDRM_BN_Clr(BN_Y);
		SDRM_BN_Clr(BN_Temp1);
		SDRM_DWD_Div(BN_Y->pData, BN_Temp1->pData, BN_G1->pData, BN_G1->Length, BN_G0->pData, BN_G0->Length);

		BN_Y->Length = BN_G1->Length;
		SDRM_BN_OPTIMIZE_LENGTH(BN_Y);

		BN_Temp1->Length = BN_G0->Length;
		SDRM_BN_Copy(BN_G1, BN_Temp1);
		SDRM_BN_OPTIMIZE_LENGTH(BN_G1);

		SDRM_BN_Clr(BN_Temp1);
		SDRM_DWD_Mul(BN_Temp1->pData, BN_Y->pData, BN_Y->Length, BN_V0->pData, BN_V0->Length);
		BN_Temp1->Length = BN_Y->Length + BN_V0->Length;
 		SDRM_BN_OPTIMIZE_LENGTH(BN_Temp1);

		SDRM_BN_Clr(BN_Temp2);
		if (SDRM_BN_Cmp(BN_V1, BN_Temp1) >= 0) {
			SDRM_BN_Add(BN_Temp2, BN_V1, BN_Temp1);
		}
		else {
			SDRM_BN_Add(BN_Temp2, BN_Temp1, BN_V1);
		}

		SDRM_BN_Copy(BN_V1, BN_Temp2);
	}

	SDRM_BN_Copy(BN_Dest, BN_Zero);
	kfree(pbBuf);

	return CRYPTO_INVERSE_NOT_EXIST;
}

/*!	\brief	Convert Montgomery number to noraml number
 * \param	BN_Dst		[out]destination, normal number
 * \param	BN_Src1		[in]source, montgomery number
 * \param	Mont		[in]montgomery parameters
 * \return	CRYPTO_SUCCESS if no error occured
 */
int SDRM_MONT_Rzn2zn(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_MONT *Mont)
{
	cc_u32			Src1_Len, Mod_Len, ri, i;
	cc_u32			carry;
	cc_u64          mod, temp;
	SDRM_BIG_NUM	*Src1 = NULL;
	
	if (!BN_Src1->Length) {
		BN_Dst->Length = 0;

		return CRYPTO_SUCCESS;
	}
	temp = Mont->ri;
	mod = do_div( temp ,SDRM_BitsInDWORD);
	Src1_Len = ri = temp;
	Mod_Len = Mont->Mod->Length;

	Src1 = SDRM_BN_Init(BN_Src1->Size + Mod_Len);
	SDRM_BN_Copy(Src1, BN_Src1);
	
	if (!Src1_Len || !Mod_Len) {
		BN_Dst->Length = 0;
		BN_Dst->pData[0] = 0;
		SDRM_BN_FREE(Src1);

		return CRYPTO_SUCCESS;
	}

	Src1->sign = BN_Src1->sign ^ Mont->Mod->sign;


	memset(Src1->pData + Src1->Length, 0, (Mod_Len + BN_Src1->Length - Src1->Length) * SDRM_SIZE_OF_DWORD);
	
	Src1->Length = Mod_Len + BN_Src1->Length;

	for (i = 0; i < Mod_Len; i++) {	
		if ((carry = SDRM_DWD_MulAdd(Src1->pData + i, Src1->Length - i, Mont->Mod->pData, Mod_Len, (cc_u32)Src1->pData[i] * Mont->N0))) {
			Src1->pData[Src1->Length++] = carry;								//Added by Park Ji soon, 05-03-06
		}																		// (cc_u32)A.pData[i]*modulus_p   <== u=a[i]*m' mod b
		  																		//  A=A+ (A.pData[i]*modulus_p* modulus[i])*b^i;
	}
	SDRM_BN_OPTIMIZE_LENGTH(Src1);

	SDRM_BN_SHR(BN_Dst, Src1, Mod_Len * 32);

	BN_Dst->Length = Src1->Length - ri;

	if (SDRM_BN_Cmp(BN_Dst, Mont->Mod) >= 0) {
		SDRM_BN_Sub(BN_Dst, BN_Dst, Mont->Mod);
	}

	SDRM_BN_FREE(Src1);

	return CRYPTO_SUCCESS;
}


/*!	\brief	Montgomery Multiplication
 * \param	BN_Dst		[out]destination, montgomery number
 * \param	BN_Src1		[in]first element, montgomery number
 * \param	BN_Src2		[in]second element, montgomery number
 * \param	Mont		[in]montgomery parameters
 * \return	CRYPTO_SUCCESS if no error occured
 */
int SDRM_MONT_Mul(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Src1, SDRM_BIG_NUM *BN_Src2, SDRM_BIG_MONT *Mont)
{
	int ret;

	SDRM_BN_Mul(BN_Dst, BN_Src1, BN_Src2);
	ret = SDRM_MONT_Rzn2zn(BN_Dst, BN_Dst, Mont);

	return ret;
}


/*!	\brief	Set Montgomery parameters
 * \param	Mont		[out]montgomery parameter
 * \param	BN_Modulus	[in]modular m
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		BN_NOT_ENOUGHT_BUFFER if malloc is failed
 * \n		CRYPTO_INVERSE_NOT_EXIST if inverse is not exists
 */
int SDRM_MONT_Set(SDRM_BIG_MONT *Mont, SDRM_BIG_NUM *BN_Modulus)
{
	SDRM_BIG_NUM	*Ri, *R;
	SDRM_BIG_NUM	*temp;
	cc_u8			*pbBuf;
	cc_u32			buf[2], dSize, dAllocSize;
	cc_u64    		mod, temp1;

	if ((Mont->Mod != NULL) && SDRM_BN_Cmp(Mont->Mod, BN_Modulus) == 0) {
		return CRYPTO_SUCCESS;
	}

	dSize = BN_Modulus->Size;
	dAllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;
	if (!(pbBuf = (cc_u8*)kmalloc((dAllocSize * 3),GFP_KERNEL))) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	Ri =   SDRM_BN_Alloc( pbBuf,				 dSize);
	if (Ri == NULL) {
		kfree(pbBuf);
		return CRYPTO_INVERSE_NOT_EXIST;
	}

	R =    SDRM_BN_Alloc((cc_u8*)Ri + dAllocSize, dSize);
	temp = SDRM_BN_Alloc((cc_u8*)R  + dAllocSize, dSize);

	if (Mont->R == NULL) {
		Mont->R = SDRM_BN_Init(BN_Modulus->Size);
	}

	if (Mont->Mod == NULL) {
		Mont->Mod = SDRM_BN_Init(BN_Modulus->Size);
	}

	SDRM_BN_Copy(Mont->Mod, BN_Modulus);
	temp1 = (SDRM_BN_num_bits(BN_Modulus) + (SDRM_BitsInDWORD - 1));
	mod = do_div(temp1 , SDRM_BitsInDWORD);
	Mont->ri = temp1 * SDRM_BitsInDWORD;
	SDRM_BN_SHL(R, BN_One, SDRM_BitsInDWORD);

	buf[0] = BN_Modulus->pData[0];
	buf[1] = 0;
	temp->pData[0] = buf[0];
	temp->Length = 1;
	temp->sign = BN_Modulus->sign;

	SDRM_BN_ModInv(Ri, R, temp);

	SDRM_BN_SHL(Ri, Ri, SDRM_BitsInDWORD);
	SDRM_BN_Sub(Ri, Ri, BN_One);
	SDRM_BN_Div(Ri, NULL, Ri, temp);
	SDRM_BN_Copy(Mont->Inv_Mod, Ri);
	Mont->N0 = Ri->pData[0];

	SDRM_BN_SHL(Mont->R, BN_One, 2 * Mont->ri);
	SDRM_BN_ModRed(Mont->R, Mont->R, Mont->Mod);

	kfree(pbBuf);

	return CRYPTO_SUCCESS;
}


/*!	\brief	Allocate new momory for Montgomery parameter
 * \param	dSize	[in]size of buffer of big number
 * \return	Pointer to created structure
 * \n		NULL if malloc failed
 */
SDRM_BIG_MONT *SDRM_MONT_Init(cc_u32 dSize)
{
	SDRM_BIG_MONT	*Mont;
	cc_u32			AllocSiz = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;

	Mont = (SDRM_BIG_MONT *)kmalloc((sizeof(SDRM_BIG_MONT) + AllocSiz * 3),GFP_KERNEL);
	if (Mont == NULL) {
		return NULL;
	}

	Mont->ri	  = 0;
	Mont->R		  = SDRM_BN_Alloc((cc_u8*)Mont + sizeof(SDRM_BIG_MONT), dSize);
	Mont->Mod	  = SDRM_BN_Alloc((cc_u8*)Mont->R + AllocSiz,		   dSize);
	Mont->Inv_Mod = SDRM_BN_Alloc((cc_u8*)Mont->Mod + AllocSiz,		   dSize);

	return Mont;
}


/*!	\brief	Free allocated memory for montgomery paramter
 * \param	Mont	[in]montgomery parameters
 * \return	void
 */
void SDRM_MONT_Free(SDRM_BIG_MONT *Mont)
{
	if (Mont != NULL) {
		kfree(Mont);
	}
}


/*!	\brief	Calc bit-length of Big Number
 * \param	BN_Src	[in]source
 * \return	bit-length
 */
int SDRM_BN_num_bits(SDRM_BIG_NUM *BN_Src)
{
	cc_u32	l;
	int		i, j;

	if (BN_Src->Length == 0) {
		return 0;
	}

	l = BN_Src->pData[BN_Src->Length - 1];
	i = (BN_Src->Length-1) * SDRM_BitsInDWORD;

	j = SDRM_UINT32_num_bits(&l);
		
	return(i + j);
}


/*!	\brief	Calc bit-length of cc_u32
 * \param	pdSrc	[in]source
 * \return	bit-length
 */
int	SDRM_UINT32_num_bits(cc_u32 *pdSrc)
{
	int		i = 0;
	cc_u32	temp;

	temp = *pdSrc;

	if (!temp) {
		return 0;
	}

	while(temp) {
		temp >>= 1;
		i++;
	}		
	
	return i;
}

/*!	\brief	Big Number Modular Exponentiation
 * \param	BN_Dst		[out]destination
 * \param	BN_Base		[in]base
 * \param	BN_Exponent	[in]exponent
 * \param	BN_Modulus	[in]modular m
 * \return	CRYPTO_SUCCESS if no error occured
 * \n		CRYPTO_MEMORY_ALLOC_FAIL	if malloc is failed
 * \n		CRYPTO_ERROR	if evaluation is failed
 */
int SDRM_BN_ModExp(SDRM_BIG_NUM *BN_Dst, SDRM_BIG_NUM *BN_Base, SDRM_BIG_NUM *BN_Exponent, SDRM_BIG_NUM *BN_Modulus)
{
	SDRM_BIG_NUM	*c_, *a_, *BN_Temp;
	SDRM_BIG_MONT	*Mont;
	int				i, m;
	cc_u8			*pbBuf;
	cc_u32			dSize, dAllocSize;

	dSize = MAX3(BN_Base->Size, BN_Exponent->Size, BN_Modulus->Size);
	dAllocSize = sizeof(SDRM_BIG_NUM) + dSize * SDRM_SIZE_OF_DWORD;

	if (!(pbBuf = (cc_u8*)kmalloc((dAllocSize * 3),GFP_KERNEL))) {
		return CRYPTO_MEMORY_ALLOC_FAIL;
	}

	c_ = SDRM_BN_Alloc(pbBuf, dSize);
	a_ = SDRM_BN_Alloc((cc_u8*)c_ + dAllocSize, dSize);
	BN_Temp = SDRM_BN_Alloc((cc_u8*)a_ + dAllocSize, dSize);

	Mont = SDRM_MONT_Init(dSize);
	SDRM_MONT_Set(Mont, BN_Modulus);

	if (SDRM_BN_Cmp(BN_Base, BN_Modulus) >= 0)
	{
		SDRM_BN_ModRed(BN_Temp, BN_Base, BN_Modulus);
	}
	else
	{
		BN_Temp = BN_Base;
	}

	if (SDRM_BN_Cmp(BN_Temp, BN_Zero) == 0)
	{
		SDRM_BN_Copy(BN_Dst, BN_Zero);
		kfree(pbBuf);
		return CRYPTO_SUCCESS;
	}

	SDRM_MONT_Zn2rzn(a_, BN_Temp, Mont);
	SDRM_MONT_Zn2rzn(c_, BN_One, Mont);

	m = SDRM_BN_num_bits(BN_Exponent);

	for (i = m - 1; i >= 0; i--) {
		SDRM_MONT_Mul(c_, c_, c_, Mont);
		
		if (SDRM_CheckBitUINT32(BN_Exponent->pData, i) == 1) {
			SDRM_MONT_Mul(c_, c_, a_, Mont);
		}
	}

	SDRM_MONT_Rzn2zn(BN_Dst, c_, Mont);

	SDRM_MONT_Free(Mont);

	kfree(pbBuf);

	return CRYPTO_SUCCESS;
}

/***************************** End of File *****************************/
