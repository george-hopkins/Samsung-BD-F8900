/**
 * \file	drm_macro.h
 * \brief	Common Macro Difinitions
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Changsup Ahn
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/08/02
 */


#ifndef _DRM_MACRO_H
#define _DRM_MACRO_H


////////////////////////////////////////////////////////////////////////////
// Header File Include
////////////////////////////////////////////////////////////////////////////
//#include <linux/stdio.h>

////////////////////////////////////////////////////////////////////////////
// Constant
////////////////////////////////////////////////////////////////////////////
#undef TRUE
#define FALSE	0
#define TRUE	( !FALSE )


////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	#define INLINE __inline
#else
	#define INLINE inline
#endif

/*!	\brief	get larger of two	*/
#define MAX2(A, B) ((A) > (B) ? (A) : (B))

/*!	\brief	get largest of three	*/
#define MAX3(C, D, E) ((C) > MAX2((D), (E)) ? (C) : MAX2((D), (E)))

/*!	\brief	print out by byte unit	*/
#undef PrintBYTE
#define PrintBYTE(msg, Data, DataLen) {					\
	int idx;											\
	printf("%10s =", msg);								\
	for( idx=0; idx<(int)DataLen; idx++) {				\
		if( (idx!=0) && ((idx%16)==0) ) printf("\n");	\
		if((idx % 4) == 0)	printf(" 0x");				\
		printf("%.2x", Data[idx]);						\
	}													\
	printf("\n");										\
}

/*!	\brief	print out in hexa representation	*/
#undef PrintBYTE_HEX
#define PrintBYTE_HEX(msg, Data, DataLen) {				\
	int idx;											\
	printf("%10s =", msg);								\
	for( idx=0; idx<(int)DataLen; idx++) {				\
		if( (idx!=0) && ((idx%8)==0) ) printf("\n");	\
		printf("0x%.2x, ", Data[idx]);					\
	}													\
	printf("\n");										\
}

/*!	\brief	print out in hexa representation without length information 	*/
#undef PrintBYTE_FILE_RAW									// raw data 형태로 사용할 수 있도록 Hex 형태로 출력 
#define PrintBYTE_FILE_RAW(pfile, Data, DataLen) {		\
	int idx;											\
	for( idx=0; idx<(int)DataLen; idx++) {				\
		if( (idx==0) || ((idx%8)!=0) )					\
			fprintf(pfile, "0x%.2x, ", Data[idx]);		\
		else											\
			fprintf(pfile, " \n0x%.2x, ", Data[idx]);	\
	}													\
}


/*!	\brief	print out message	*/
#undef PrintMSG
#define PrintMSG(msg) {									\
	fprintf(stdout, "\n************************************************\n");	\
	fprintf(stdout, "*     %s\n", msg);					\
	fprintf(stdout, "*\n");								\
}


/*!	\brief	copy 16 byte block	*/
#undef BlockCopy
#define BlockCopy(pbDst, pbSrc) {						\
	memcpy(pbDst, pbSrc, 16);							\
}

/*!	\brief	xor 16 byte block	*/
#undef BlockXor
#define BlockXor(pbDst, phSrc1, phSrc2) {				\
	int idx;											\
	for(idx = 0; idx < 16; idx++)						\
		(pbDst)[idx] = (phSrc1)[idx] ^ (phSrc2)[idx];	\
}


/*!	\brief	convert 32-bit unit to 4 byte	*/
#undef GET_UINT32
#define GET_UINT32(n,b,i)								\
{														\
    (n) = ((unsigned int)((b)[(i)    ]) << 24 )			\
        | ((unsigned int)((b)[(i) + 1]) << 16 )			\
        | ((unsigned int)((b)[(i) + 2]) <<  8 )			\
        | ((unsigned int)((b)[(i) + 3])       );		\
}

/*!	\brief	4 byte to 32-bit unit	*/
#undef PUT_UINT32
#define PUT_UINT32(n,b,i)								\
{														\
    (b)[(i)    ] = (UINT8) ( (n) >> 24 );				\
    (b)[(i) + 1] = (UINT8) ( (n) >> 16 );				\
    (b)[(i) + 2] = (UINT8) ( (n) >>  8 );				\
    (b)[(i) + 3] = (UINT8) ( (n)       );				\
}

/*!	\brief	convert 24-bit unit to 3 byte	*/
#undef GET_UINT24
#define GET_UINT24(n,b,i)								\
{														\
    (n) = ( (b)[(i)    ] << 16 )						\
        | ( (b)[(i) + 1] << 8  )						\
        | ( (b)[(i) + 2]       );						\
}

/*!	\brief	convert 3 byte to 24-bit unit	*/
#undef PUT_UINT24
#define PUT_UINT24(n,b,i)								\
{														\
    (b)[(i)    ] = (UINT8) ( (n) >> 16 );				\
    (b)[(i) + 1] = (UINT8) ( (n) >>  8 );				\
    (b)[(i) + 2] = (UINT8) ( (n) >>    );				\
}


/*!	\brief	convert 16-bit unit to 2 byte	*/
#undef GET_UINT16
#define GET_UINT16(n,b,i)								\
{														\
    (n) = ( (b)[(i)    ] << 8 )							\
        | ( (b)[(i) + 1]	);							\
}

/*!	\brief	convert 2 byte to 16-bit unit	*/
#undef PUT_UINT16
#define PUT_UINT16(n,b,i)								\
{														\
    (b)[(i)    ] = (UINT8) ( (n) >> 8 );				\
    (b)[(i) + 1] = (UINT8) ( (n)      );				\
}


/*!	\brief	read 1 byte of s form o & increase o	*/
#undef READ_8
#define READ_8(t,s,o) {									\
	t = (UINT8) s[o];									\
	o+=1;												\
}

/*!	\brief	read 2 byte of sfrom o & increase o	*/
#undef READ_16
#define READ_16(t,s,o) {								\
	GET_UINT16(t,s,o);									\
	o+=2;												\
}

/*!	\brief	read 3 byte of s from o & increase o	*/
#undef READ_24
#define READ_24(t,s,o) {								\
	GET_UINT24(t,s,o);									\
	o+=3;												\
}

/*!	\brief	read 4 byte of s from o & increase o	*/
#undef READ_32
#define READ_32(t,s,o) {								\
	GET_UINT32(t,s,o);									\
	o+=4;												\
}

/*!	\brief	write 4 byte to s from o & increase o	*/
#undef WRITE_32
#define WRITE_32(t,s,o) {								\
	PUT_UINT32(s,t,o);									\
	o+=4;												\
}

/*!	\brief	write 3 byte to s from o & increase o	*/
#undef WRITE_24
#define WRITE_24(t,s,o) {								\
	PUT_UINT24(s,t,o);									\
	o+=3;												\
}

/*!	\brief	write 2 byte to s from o & increase o	*/
#undef WRITE_16
#define WRITE_16(t,s,o) {								\
	PUT_UINT16(s,t,o);									\
	o+=2;												\
}

/*!	\brief	write 1 byte to s from o & increase o	*/
#undef WRITE_8
#define WRITE_8(t,s,o) {								\
	t[o] = (UINT8)s;									\
	o+=1;												\
}

#endif

/***************************** End of File *****************************/
