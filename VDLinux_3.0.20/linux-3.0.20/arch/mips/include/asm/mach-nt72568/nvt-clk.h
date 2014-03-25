/**
 * @mainpage clock generator driver in kernel mode
 *
 * Copyright (c) Novatek Microelectronics Corp., Ltd. All Rights Reserved.
 *
 * @section Desscription
 *
 * Documation of clock generator APIs and type definitions.
 */

/**
 * @file arch/mips/include/asm/ntclk.h
 *
 * @author NOVATEK SP/KSW
 */
#ifndef __NVT_CLK_H__
#define __NVT_CLK_H__

#include <linux/module.h>
#include <linux/types.h>

/**
 * @ingroup type
 *
 * @defgroup AHBPLL Enumeration of AHB clock clock source
 * @{
 *
 * @typedef EN_SYS_AHB_CLK_SRC
 *
 * A enumeration for AHB clock source select 
 */
typedef enum _EN_SYS_AHB_CLK_SRC
{
	EN_SYS_AHB_CLK_SRC_MIN = 0,								///< Minimum value of EN_SYS_AHB_CLK_SRC, don't add entry before this

	///< AHB clock source select
    EN_SYS_AHB_CLK_SRC_MIPS_D6CK  = EN_SYS_AHB_CLK_SRC_MIN,	///< 3'b000: (MIPS MPLL)/6
    EN_SYS_AHB_CLK_SRC_MIPS_D4CK,							///< 3'b001: (MIPS MPLL)/4
    EN_SYS_AHB_CLK_SRC_BODA_CK,								///< 3'b010: BODA MPLL
    EN_SYS_AHB_CLK_SRC_AXI_CK,								///< 3'b011: AXI MPLL
    EN_SYS_AHB_CLK_SRC_REF_96M,								///< 3'b1xx: (Reference Clock 12M) * 8

	///< \n
	EN_SYS_AHB_CLK_SRC_MAX = 7,								///< Maximum value of EN_SYS_AHB_CLK_SRC, don't add entry before this

} EN_SYS_AHB_CLK_SRC;
//@}

/**
 * @ingroup type
 *
 * @defgroup MPLL Index of MPLL type
 * @{
 *
 * @var EN_SYS_CLK_MPLL_TYPE
 *
 * Index of MPLL TYPE
 *
 * @hideinitializer
 */
typedef enum _EN_SYS_CLK_MPLL_TYPE
{
	EN_SYS_CLK_MPLL_MIN = 0,			///< Minimum value of EN_SYS_CLK_MPLL_TYPE, don't add entry before this !!

	///< \n
	EN_SYS_CLK_MPLL_MIPS	=	0x62,	///< MIPS
	EN_SYS_CLK_MPLL_DSP		=	0x75,	///< DSP
	EN_SYS_CLK_MPLL_AXI		=	0x6a,	///< AXI
	EN_SYS_CLK_MPLL_DDR		=	0x82,	///< DDR
	EN_SYS_CLK_MPLL_MPG		=	0x9a,	///< MPG(Boda)

	///< \n
	EN_SYS_CLK_MPLL_MAX,  				///< Maximum value of EN_SYS_CLK_MPLL_TYPE, don't add entry after this !!

} EN_SYS_CLK_MPLL_TYPE;
//@}

/**
 * @defgroup funcs Function prototype
 * @{
 */

/**
 * @fn u32 SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_TYPE enType)
 *
 * @brief Get System MPLL (Hz)
 *
 * @param[in] enType	MPLL which want to get 
 *
 * @return Value of MPLL (Hz)
 *
 */
u32 SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_TYPE enType);

/**
 * @fn void SYS_CLK_SetMpll(EN_SYS_CLK_MPLL_TYPE enType, u32 u32Freq)
 *
 * @brief Set MPLL (Hz)
 *
 * @param[in] enType	MPLL type which want to set 
 * @param[in] u32Freq	MPLL value which want to set 
 *
 */
void SYS_CLK_SetMpll(EN_SYS_CLK_MPLL_TYPE enType, u32 u32Freq);

//@}

#endif /* !__NVT_CLK_H__*/

