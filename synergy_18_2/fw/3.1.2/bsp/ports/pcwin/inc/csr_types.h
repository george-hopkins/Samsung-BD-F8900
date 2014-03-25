#ifndef CSR_TYPES_H__
#define CSR_TYPES_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef  FALSE
#define FALSE (0)

#undef  TRUE
#define TRUE (1)

/* Basic types */
typedef size_t CsrSize;         /* Return type of sizeof (ISO/IEC 9899:1990 7.1.6) */
typedef ptrdiff_t CsrPtrdiff;   /* Type of the result of subtracting two pointers (ISO/IEC 9899:1990 7.1.6) */
typedef CsrSize CsrUintptr;     /* Unsigned integer large enough to hold any pointer (ISO/IEC 9899:1999 7.18.1.4) */
typedef CsrPtrdiff CsrIntptr;   /* Signed integer large enough to hold any pointer (ISO/IEC 9899:1999 7.18.1.4) */

/* Unsigned fixed width types */
typedef unsigned char CsrUint8;
typedef unsigned short CsrUint16;
typedef unsigned int CsrUint32;

/* Signed fixed width types */
typedef signed char CsrInt8;
typedef signed short CsrInt16;
typedef signed int CsrInt32;

/* Boolean */
typedef CsrUint8 CsrBool;

/* String types */
typedef char CsrCharString;
typedef CsrUint8 CsrUtf8String;
typedef CsrUint16 CsrUtf16String;   /* 16-bit UTF16 strings */
typedef CsrUint32 CsrUint24;

/*
 * 64-bit integers
 *
 * Note: If a given compiler does not support 64-bit types, it is
 * OK to omit these definitions;  32-bit versions of the code using
 * these types may be available.  Consult the relevant documentation
 * or the customer support group for information on this.
 */
#define CSR_HAVE_64_BIT_INTEGERS
typedef unsigned __int64 CsrUint64;
typedef signed __int64 CsrInt64;

/*
 * Floating point
 *
 * Note: If a given compiler does not support floating point, it is
 * OK to omit these definitions;  alternative versions of the code using
 * these types may be available.  Consult the relevant documentation
 * or the customer support group for information on this.
 */
#define CSR_HAVE_FLOATING_POINT
typedef float CsrFloat;
typedef double CsrDouble;

#ifdef __cplusplus
}
#endif

#endif
