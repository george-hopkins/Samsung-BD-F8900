#ifndef _TYPES_H_
#define _TYPES_H_
typedef unsigned int		BOOL;
typedef unsigned char		BOOLEAN;
typedef unsigned char  	    U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef signed char 		S8;
typedef signed short		S16;
typedef signed int 			S32;
typedef unsigned char  	    INT8U;
typedef signed   char  		INT8S;
typedef unsigned short 	    INT16U;
typedef signed   short 		INT16S;
typedef unsigned int   		INT32U;
typedef signed   int   		INT32S;
typedef float          		FP32;
typedef void				VOID;
typedef unsigned int   		INT24U;
typedef unsigned char   	UCHAR;

#ifdef CODESONAR_INCLUDE
#define ALIGN_FOUR 
#else  // CODESONAR_INCLUDE
#define ALIGN_FOUR 			__align(4)
#endif // CODESONAR_INCLUDE

#endif

