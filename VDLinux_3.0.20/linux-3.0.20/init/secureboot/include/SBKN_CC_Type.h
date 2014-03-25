/**
 * \file	CC_Type.h
 * \brief	data types for CryptoCore library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Jisoon Park
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2008/08/26
 */


#ifndef _CC_TYPE_H_
#define _CC_TYPE_H_

/*!	\brief	1-byte data type	*/
typedef		unsigned char						cc_u8;

/*!	\brief	2-byte data type	*/
typedef		unsigned short						cc_u16;

/*!	\brief	4-byte data type	*/
typedef		unsigned int						cc_u32;

#ifndef _OP64_NOTSUPPORTED

/*!	\brief	8-byte data type	*/
#ifdef _WIN32
	typedef		unsigned __int64				cc_u64;
#else
	typedef		unsigned long long				cc_u64;
#endif		//_WIN32

#endif		//_OP64_NOTSUPPORTED

#endif		//_CC_TYPE_H_

/***************************** End of File *****************************/
