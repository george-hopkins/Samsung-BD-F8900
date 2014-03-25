/**
 * \file	CryptoCore.h
 * \brief	main header file of crypto library
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : 
 * Dept : DRM Lab, Digital Media Laboratory
 * Creation date : 2006/08/02
 */


#ifndef _CRYPTOCORE_H
#define _CRYPTOCORE_H

#ifdef _USRDLL
	#if defined(CRYPTOLIB_EXPORTS)
		#define ECRYPTO_API __declspec(dllexport)
	#elif defined(CRYPTOLIB_IMPORTS)
		#define ECRYPTO_API __declspec(dllimport)
	#else
		#define ECRYPTO_API
	#endif
#else
	#define ECRYPTO_API
#endif



////////////////////////////////////////////////////////////////////////////
// Header File Include
////////////////////////////////////////////////////////////////////////////
#include <linux/slab.h>
#include <asm/div64.h>
#include <linux/time.h>
#include "SBKN_CC_Type.h"
#include "SBKN_drm_macro.h"
#include "SBKN_CC_Constants.h"
#include "SBKN_CC_Context.h"

////////////////////////////////////////////////////////////////////////////
// Global Variable
////////////////////////////////////////////////////////////////////////////
/*!	\brief	Log CTX variable	*/
//extern Log4DRM_CTX ECRYPTO_API CryptoLogCTX;

#endif

/***************************** End of File *****************************/
