/*
 * Copyright (C) 2011  Novatek , Inc.
 *	All rights reserved.
 */

#include <linux/module.h>
#include <linux/spinlock.h>

#include <asm/wbflush.h>

#include <nvt-clk.h>
#include <nvt-sys.h>

#define _PBUS_SetData(u8Offset, u8Value) \
({ \
	SYS_MPLL_PBUS_OFFSET = u8Offset; \
	SYS_MPLL_PBUS_WR_DATA = u8Value; \
 })

#define _PBUS_GetData(u8Offset) \
({ \
	SYS_MPLL_PBUS_OFFSET = u8Offset; \
	SYS_MPLL_PBUS_RD_DATA; \
 })

#define _PBUS_EnablePageB()	\
	SYS_MPLL_PBUS_PAGE_EN = _SYS_MPLL_PAGE_B_EN;

#define _PBUS_EnablePage0()	\
	SYS_MPLL_PBUS_PAGE_EN = _SYS_MPLL_PAGE_0_EN;

DEFINE_RAW_SPINLOCK(clock_gen_lock);

/**
 * @fn u32 SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_TYPE enType)
 *
 * @brief Get MPLL (Hz)
 *
 * @param[in] enType	MPLL which want to get 
 *
 * @return Value of MPLL (Hz)
 *
 */
u32 SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_TYPE enType)
{
	u32 u32Mpll = 0;
	unsigned long flags;

	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	//!< Select page and set MPLL in crirical section
	_PBUS_EnablePageB();

	u32Mpll |= (_PBUS_GetData(enType + 0) << 0);
	u32Mpll |= (_PBUS_GetData(enType + 1) << 8);
	u32Mpll |= (_PBUS_GetData(enType + 2) << 16);

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);

	u32Mpll *= 12;
	u32Mpll >>= 17;
	u32Mpll *= 1000000;

	return u32Mpll;
}

/**
 * @fn void SYS_CLK_SetMpll(EN_SYS_CLK_MPLL_TYPE enType, u32 u32Val)
 *
 * @brief Set MPLL (Hz)
 *
 * @param[in] enType	MPLL type which want to set 
 * @param[in] u32Val	MPLL value which want to set 
 *
 */
void SYS_CLK_SetMpll(EN_SYS_CLK_MPLL_TYPE enType, u32 u32Val)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	//!< Select page and set MPLL in crirical section
	_PBUS_EnablePageB();

	_PBUS_SetData((enType + 0), (u32Val >> 0) & 0xff);
	_PBUS_SetData((enType + 1), (u32Val >> 8) & 0xff);
	_PBUS_SetData((enType + 2), (u32Val >> 16) & 0xff);

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);
}

EXPORT_SYMBOL(SYS_CLK_GetMpll);
EXPORT_SYMBOL(SYS_CLK_SetMpll);
EXPORT_SYMBOL(clock_gen_lock);

