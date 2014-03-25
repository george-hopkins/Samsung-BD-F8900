/**
 *     @mainpage   Nestle Layer
 *
 *     @section Intro
 *       TFS5 File system's VFS framework
 *   
 *     @MULTI_BEGIN@ @COPYRIGHT_DEFAULT
 *     @section Copyright COPYRIGHT_DEFAULT
 *            COPYRIGHT. SAMSUNG ELECTRONICS CO., LTD.
 *                                    ALL RIGHTS RESERVED
 *     Permission is hereby granted to licensees of Samsung Electronics Co., Ltd. products
 *     to use this computer program only in accordance 
 *     with the terms of the SAMSUNG FLASH MEMORY DRIVER SOFTWARE LICENSE AGREEMENT.
 *     @MULTI_END@
 *
 */

/**
 * @brief	About the byte-ordering
 * @file	ess_endian.h
 * @author	
 */

#ifndef __ESS_ENDIAN_H__
#define	__ESS_ENDIAN_H__

#define BYTE_SWAP_16BIT(x)				\
			((((x) & 0xFF00) >> 8) |		\
			 (((x) & 0x00FF) << 8))

/** Swap the byte-order */
#define	BYTE_SWAP_32BIT(x)				\
			((((x) & 0xFF000000) >> 24) |	\
			 (((x) & 0x00FF0000) >>  8) |	\
			 (((x) & 0x0000FF00) <<  8) |	\
			 (((x) & 0x000000FF) << 24))	 

#define BYTE_SWAP_64BIT(x)				\
			(((x >> 56) | \
			((x >> 40) & 0xff00) | \
			((x >> 24) & 0xff0000) | \
			((x >> 8) & 0xff000000) | \
			((x << 8) & (0xfful << 32)) | \
			((x << 24) & (0xfful << 40)) | \
			((x << 40) & (0xfful << 48)) | \
			((x << 56))))			 

#endif // end of __ENDIAN_H__
