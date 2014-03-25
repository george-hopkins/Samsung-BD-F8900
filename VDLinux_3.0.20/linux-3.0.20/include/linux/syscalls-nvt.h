/**
 * @mainpage clock generator driver in user mode
 *
 * Copyright (c) Novatek Microelectronics Corp., Ltd. All Rights Reserved.
 *
 * @section Desscription
 *
 * Documation of clock generator APIs and type definitions.
 */

/**
 * @file include/linux/syscalls-nvt.h
 *
 * @author NOVATEK SP/KSW
 */
#ifndef __SYSCALL_NVT_H__
#define __SYSCALL_NVT_H__

#include <linux/unistd.h>
#include <linux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup type Type definitions.
 */
/**
 * @defgroup struct Struct definitions.
 */

/**
 * @ingroup type
 *
 * @defgroup AUDPLL Enumeration of AUDPLL clock source
 * @{
 *
 * @typedef EN_DRV_AUDPLL_CLK_SRC
 *
 * A structure for audio clock source select 
 */
typedef enum _EN_DRV_AUDPLL_CLK_SRC
{
    EN_DRV_AUDPLL_CLK_SRC_SIF32K  = 0, // sif 32K
    EN_DRV_AUDPLL_CLK_SRC_HDMI32K = 1, // hdmi pll 512x32K
    EN_DRV_AUDPLL_CLK_SRC_HDMI44K = 2, // hdmi pll 512x44.1K
    EN_DRV_AUDPLL_CLK_SRC_HDMI48K = 3, // hdmi pll 512x48K
    EN_DRV_AUDPLL_CLK_SRC_12M     = 4, // 12M
    EN_DRV_AUDPLL_CLK_SRC_IISIN   = 5, // iisin

    EN_DRV_AUDPLL_CLK_SRC_NUM

} EN_DRV_AUDPLL_CLK_SRC;
//@}

/**
 * @ingroup type
 *
 * @defgroup I3CLK Enumeration of I3 clock source
 * @{
 *
 * @typedef EN_DRV_CLK_TOP_I3
 *
 * for EN_DRV_CLK_TOP_I3 
 */
typedef enum _EN_DRV_CLK_TOP_I3
{
	EN_DRV_CLK_SRC_I3_VD        = 0,  ///< 4'b0000 vd clk 27M
	EN_DRV_CLK_SRC_I3_CRC       = 1,  ///< 4'b0001 crc clk 27M
	EN_DRV_CLK_SRC_I3_CPU_D4    = 2,  ///< 4'b0010 mips clock div 4
	EN_DRV_CLK_SRC_I3_CPU_D6    = 3,  ///< 4'b0011 mips clock div 6
	EN_DRV_CLK_SRC_I3_CPU_D2    = 4,  ///< 4'b0100 axi clock div 1.5
	EN_DRV_CLK_SRC_I3_AXI_D2    = 5,  ///< 4'b0101 axi clock div 2
	EN_DRV_CLK_SRC_I3_VD4X      = 6,  ///< 4'b0110 vd-4xclk 108M
	EN_DRV_CLK_SRC_I3_DP        = 7,  ///< 4'b0111 display pixel clock
	EN_DRV_CLK_SRC_I3_DP_D2     = 8,  ///< 4'b1xxx display pixel clock div 2
} EN_DRV_CLK_TOP_I3;
//@}

/**
 * @ingroup type
 *
 * @defgroup SDADCCLK Enumeration of SIF DEMOD ADC clock source
 * @{
 *
 * @typedef EN_DRV_CLK_TOP_SDADC
 *
 * for EN_DRV_CLK_TOP_SDADC 
 */
typedef enum _EN_DRV_CLK_TOP_SDADC
{
    EN_DRV_CLK_SRC_SDADC_SIF    	= 0, ///< 2'b00 clock from MPLL VD_CK 27M (for sif)
    EN_DRV_CLK_SRC_SDADC_DEMOD  	= 1, ///< 2'b01 clock from MPLL DMD_CK (for demod)	
    EN_DRV_CLK_SRC_SDADC_DEMOD_D4 	= 2, ///< 2'b1x clock from MPLL DMD_CK_D4 (for demod)	
} EN_DRV_CLK_TOP_SDADC;
//@}

/**
 * @ingroup type
 *
 * @defgroup TSCLK Enumeration of TS0/TS1 clock source
 * @{
 *
 * @typedef EN_DRV_CLK_TOP_TS
 *
 * for EN_DRV_CLK_TOP_TS 
 */
typedef enum _EN_DRV_CLK_TOP_TS
{
    EN_DRV_TS_CS_TSIF          = 0, ///< 2'b00 clock from demod tsif mpclk
    EN_DRV_TS_CS_TSI           = 1, ///< 2'b01 clock from tsi clock padin
    EN_DRV_TS_CS_TSS           = 2, ///< 2'b1x clock from tss clock padin
} EN_DRV_CLK_TOP_TS;
//@}

/**
 * @ingroup struct
 * @{
 *
 * @union UN_DRV_CLK_SETTING
 *
 * A structure to store information for updating MPLL
 */
typedef union 
{
	/**
	 *  Normal MPLL frequency
	 */
	u32 u32FreqHz;

	/**
	 *  A structure for AUD/CRC MPLL
	 */
	struct
	{
		u32 u32Mul;
		u32 u32Div;
		u32 u32CtrlReg;
		u32 u32RefckMux;
		EN_DRV_AUDPLL_CLK_SRC enClkSrc;
	} stAUPLL;

	/**
	 *  A structure for VD MPLL
	 */
	struct
	{
		u32 u32Mul;
		u32 u32Div;
		u32 u32RefckMux;
	} stVDPLL;

	/**
	 * A structure for DP/DDR SSC MPLL 
	 */
	struct
	{
		u32 u32Enable;
		u32 u32FreqKHz;
		u32 u32PercentX10;
	} stSSC;
} UN_DRV_CLK_SETTING;
//@}


/**
 * @ingroup type
 *
 * @defgroup MPLL Index of MPLL type
 * @{
 *
 * @var EN_DRV_CLK_MPLL_TYPE
 *
 * Index of MPLL TYPE
 *
 * @hideinitializer
 */
typedef enum _EN_DRV_CLK_MPLL_TYPE
{
	EN_DRV_CLK_MPLL_MIN = 0,					///< Minimum value of EN_DRV_CLK_MPLL_TYPE, don't add entry before this !!

	///< \n
	EN_DRV_CLK_MPLL_MIPS = EN_DRV_CLK_MPLL_MIN, ///< MIPS
	EN_DRV_CLK_MPLL_DSP,						///< DSP
	EN_DRV_CLK_MPLL_AXI,						///< AXI
	EN_DRV_CLK_MPLL_DDR,						///< DDR
	EN_DRV_CLK_MPLL_VD,							///< VD
	EN_DRV_CLK_MPLL_CR27,						///< CR27
	EN_DRV_CLK_MPLL_MPG,						///< MPG(Boda)
	EN_DRV_CLK_MPLL_DMD,						///< DMD
	EN_DRV_CLK_MPLL_ETH,						///< ETH
	EN_DRV_CLK_MPLL_USB,						///< USB
	EN_DRV_CLK_MPLL_ADAC,						///< ADAC
	EN_DRV_CLK_MPLL_DP,							///< DP
	EN_DRV_CLK_MPLL_AU,     					///< AU
	EN_DRV_CLK_MPLL_CRC,    				 	///< CRC
	EN_DRV_CLK_MPLL_DP_SSC, 					///< DP SSC
	EN_DRV_CLK_MPLL_DDR_SSC,					///< DDR SSC

	///< \n
	EN_DRV_CLK_MPLL_MAX,  						///< Maximum value of EN_DRV_CLK_MPLL_TYPE, don't add entry after this !!

} EN_DRV_CLK_MPLL_TYPE;
//@}

/**
 * @ingroup type
 *
 * @defgroup Inv Emueration of clock inverse
 * @{
 *
 * @var EN_DRV_CLK_INV
 *
 * The enumeration of clock inverse 
 *
 * @hideinitializer
 */
typedef enum _EN_DRV_CLK_INV
{
	EN_DRV_CLK_INV_MIN = 0,									///< Minimum value of EN_DRV_CLK_INV, don't add entry before this !!

	///< ATV clock & select (0x40)
	EN_DRV_CLK_INV_AUDIO_HDMI_27M	= ((0x40 << 3) + 1),	///< bit[1]

	///< Engine clock select (0x44)
	EN_DRV_CLK_INV_MAC0 			= ((0x44 << 3) + 6),	///< bit[6]
	EN_DRV_CLK_INV_MAC1 			= ((0x44 << 3) + 13),	///< bit[13]
	EN_DRV_CLK_INV_MAC_OUT,									///< bit[14]
	EN_DRV_CLK_INV_ATV_VCLK			= ((0x44 << 3) + 17),	///< bit[17]
	EN_DRV_CLK_INV_SIF_VCLK			= ((0x44 << 3) + 20),	///< bit[20]

	///< Engine clock select (0x48)
	EN_DRV_CLK_INV_TS_27M			= ((0x48 << 3) + 5),	///< bit[5]
	EN_DRV_CLK_INV_TVDAC			= ((0x48 << 3) + 8),	///< bit[8]
	EN_DRV_CLK_INV_SIF_ADC 			= ((0x48 << 3) + 9),	///< bit[9]
	EN_DRV_CLK_INV_TS0				= ((0x48 << 3) + 12),	///< bit[12]
	EN_DRV_CLK_INV_TS1				= ((0x48 << 3) + 15),	///< bit[15]
	EN_DRV_CLK_INV_I2S_IN_MCLK 		= ((0x48 << 3) + 16),	///< bit[16]
	EN_DRV_CLK_INV_I2S_OUT_A_MCLK,							///< bit[17]	
	EN_DRV_CLK_INV_I2S_OUT_A_BCLK,							///< bit[18]
	EN_DRV_CLK_INV_I2S_OUT_B_MCLK,							///< bit[19]
	EN_DRV_CLK_INV_I2S_OUT_B_BCLK,							///< bit[20]
	EN_DRV_CLK_INV_I2S_OUT_C_MCLK,							///< bit[21]
	EN_DRV_CLK_INV_I2S_OUT_C_BCLK,							///< bit[22]
	EN_DRV_CLK_INV_CCIR				= ((0x48 << 3) + 23),	///< bit[23]
	EN_DRV_CLK_INV_AUD_DAC0,								///< bit[24]			
	EN_DRV_CLK_INV_AUD_DAC1,								///< bit[25]			
	EN_DRV_CLK_INV_AUD_ADC,									///< bit[26]
	EN_DRV_CLK_INV_I2S_BCLK_BLOCK	= ((0x48 << 3) + 25),	///< bit[30]

	///< Engine clock select (0x4C)
	EN_DRV_CLK_INV_HDMI_SIF_MCLK	= ((0x4C << 3) + 0),	///< bit[0]
	EN_DRV_CLK_INV_HDMI_AUD_MCLK, 							///< bit[1]
	EN_DRV_CLK_INV_HDMI_VOUTODCK, 							///< bit[2]
	EN_DRV_CLK_INV_TVADC_RDCLK_CAP,							///< bit[3]
	EN_DRV_CLK_INV_TVADC_RDCLK_R,							///< bit[4]
	EN_DRV_CLK_INV_TVADC_RDCLK_G,							///< bit[5]
	EN_DRV_CLK_INV_TVADC_RDCLK_B,							///< bit[6]
	EN_DRV_CLK_INV_2DTO3D,									///< bit[7]
	EN_DRV_CLK_INV_DIVF_27M			= ((0x4C << 3) + 9),	///< bit[9]
	EN_DRV_CLK_INV_DIVF_108M,								///< bit[10]
	EN_DRV_CLK_INV_CCIR_IN,									///< bit[11]			
	EN_DRV_CLK_INV_LED_SPI,									///< bit[12]			
	EN_DRV_CLK_INV_TCON_LVDS		= ((0x4C << 3) + 15),	///< bit[15]
	EN_DRV_CLK_INV_ADC_BIST			= ((0x4C << 3) + 22),	///< bit[22]
	EN_DRV_CLK_INV_TVADC0_DATA,								///< bit[23]
	EN_DRV_CLK_INV_SIFADC_DATA,								///< bit[24]	

	///< \n
	EN_DRV_CLK_INV_MAX				= ((0x4C << 3) + 31),	///< Maximum value of EN_DRV_CLK_INV, don't add entry after this
} EN_DRV_CLK_INV;
//@}

/**
 * @ingroup type
 *
 * @defgroup Src Emueration of clock source
 * @{
 *
 * @var EN_DRV_CLK_SRC
 *
 * The enumeration of clock source
 *
 * @hideinitializer
 */
typedef enum _EN_DRV_CLK_SRC
{
	EN_DRV_CLK_SRC_MIN = 0,						///< Minimum value of EN_DRV_CLK_SRC, don't add entry before this

	///< System clock select (0x30)
	EN_DRV_CLK_SRC_MIPS = EN_DRV_CLK_SRC_MIN,	///< bit[1:0]
	EN_DRV_CLK_SRC_DSP,							///< bit[4:2]
	EN_DRV_CLK_SRC_AHB,							///< bit[7:5]
	EN_DRV_CLK_SRC_AXI,							///< bit[10:8]
                                                     
	///< System clock select (0x34)                  
	EN_DRV_CLK_SRC_MIPS_CHG_EN,					///< bit[0]
	EN_DRV_CLK_SRC_DSP_CHG_EN,					///< bit[1]
	EN_DRV_CLK_SRC_AHB_DIV2_EN,					///< bit[2]
	EN_DRV_CLK_SRC_MIPS_CHG_BYPASS_EN,			///< bit[4]
	EN_DRV_CLK_SRC_DSP_CHG_BYPASS_EN,			///< bit[5]
	EN_DRV_CLK_SRC_MIPS_CHG_ACK_EN,				///< bit[6]
	EN_DRV_CLK_SRC_MAU_MCLK,					///< bit[7]
                                                     
	///< Device clock select (0x38)				     
	EN_DRV_CLK_SRC_TVE_DCLK,					///< bit[0]
	EN_DRV_CLK_SRC_I3,							///< bit[4:1]
	EN_DRV_CLK_SRC_USBPHY12M,					///< bit[6:5]
	EN_DRV_CLK_SRC_WDOG_DIV,					///< bit[16:7]
	EN_DRV_CLK_SRC_LEON,						///< bit[19:17]
	EN_DRV_CLK_SRC_HDMI,						///< bit[21:20]
	EN_DRV_CLK_SRC_ATV_VD,						///< bit[22]
	EN_DRV_CLK_SRC_TVE_VD,						///< bit[23]
	EN_DRV_CLK_SRC_TVDAC_VD,					///< bit[25:24]
	EN_DRV_CLK_SRC_I3_DELAY_EN,					///< bit[26]
                                                     
	///< Test mode, clock select (0x3C)              
                                                     
	///< ATV clock & select (0x40)                   
	EN_DRV_CLK_SRC_ATV_CFG_EN,					///< bit[23:16]
	EN_DRV_CLK_SRC_ATV_CFG_RST_PWD,				///< bit[31:24]
                                                     
	///< Engine clock select (0x44)                  
	EN_DRV_CLK_SRC_MAC_PADIN,					///< bit[0]
	EN_DRV_CLK_SRC_MAC0_DELAY,					///< bit[5:1]
	EN_DRV_CLK_SRC_MAC1_DIV2_EN,				///< bit[7], for MAC clock from pad-in
	EN_DRV_CLK_SRC_MAC1_DELAY,					///< bit[12:8]
	EN_DRV_CLK_SRC_MAC_OUTPUT_CLK_EN,			///< bit[15]
	EN_DRV_CLK_SRC_ATV_VCLK_27M,				///< bit[16]
	EN_DRV_CLK_SRC_AUD_SIF_27M,					///< bit[18]
	EN_DRV_CLK_SRC_SIF_VCLK_27M,				///< bit[19]
	EN_DRV_CLK_SRC_IP_TSCLK,					///< bit[26:21]
	EN_DRV_CLK_SRC_DVIF_TSCLK, 					///< bit[28:27]
	EN_DRV_CLK_SRC_LVDS_PCLK_PADIN,				///< bit[29]
	EN_DRV_CLK_SRC_MACRO,						///< bit[31:30]
                                                     
	///< Engine clock select (0x48)                  
	EN_DRV_CLK_SRC_SDADC,						///< bit[2:0]
	EN_DRV_CLK_SRC_SIF_MCLK,					///< bit[3]
	EN_DRV_CLK_SRC_TS_27M,						///< bit[4]
	EN_DRV_CLK_SRC_TVADC,						///< bit[7:6]
	EN_DRV_CLK_SRC_TS0,							///< bit[11:10]
	EN_DRV_CLK_SRC_TS1,							///< bit[14:13]
	EN_DRV_CLK_SRC_USB_OSC,						///< bit[27]
	EN_DRV_CLK_SRC_SIF,							///< bit[29:28]
                                                     
	///< Engine clock select (0x4C)                  
	EN_DRV_CLK_SRC_DIVF,						///< bit[8]
	EN_DRV_CLK_SRC_SPDIF,						///< bit[14:13]
	EN_DRV_CLK_SRC_ADC_BIST,					///< bit[21:19]
	EN_DRV_CLK_SRC_TCON_OSC,					///< bit[25]
	EN_DRV_CLK_SRC_TCON_SPI_DIV,				///< bit[29:26]

	///< \n
	EN_DRV_CLK_SRC_MAX,							///< Maximum value of EN_DRV_CLK_SRC, don't add entry after this
} EN_DRV_CLK_SRC;
//@}


typedef struct _ST_DRV_CLK_SRC_SEL
{
	u32 u32RegOff;
	u32 u32FieldSize;
	u32 u32FieldPos;
} ST_DRV_CLK_SRC_SEL;

typedef struct _ST_DRV_CLK_RST
{
	u32 u32RegOff;
	u32 u32Mask;
} ST_DRV_CLK_RST;


/**
 * @ingroup type
 *
 * @defgroup MASK Emueration of clock mask
 * @{
 *
 * @var EN_DRV_CLK_MASK
 *
 * The enumeration of clock mask
 *
 * @hideinitializer
 */
typedef enum _EN_DRV_CLK_MASK
{
	EN_DRV_CLK_MASK_MIN = 0,									///< Minimum value of EN_DRV_CLK_MASK, don't add entry before this

	///< Partition A, AHB clock mask (0x00)
	EN_DRV_CLK_MASK_AHB_AUD				= ((0x00 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AHB_MAC,  									///< bit[1]
	EN_DRV_CLK_MASK_AHB_TVE,									///< bit[2]
	EN_DRV_CLK_MASK_AHB_DEMOD,									///< bit[3]
	EN_DRV_CLK_MASK_AHB_LZD,									///< bit[4]
	EN_DRV_CLK_MASK_AHB_VIF,									///< bit[5]
	EN_DRV_CLK_MASK_AHB_AC_DATA,								///< bit[6]
	EN_DRV_CLK_MASK_AHB_TSIF,									///< bit[7]
	EN_DRV_CLK_MASK_AHB_TESTPBUS,								///< bit[8]
	
	///< Partition B, AHB clock mask (0x04) 
	EN_DRV_CLK_MASK_AHB_TS				= ((0x04 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AHB_CI,										///< bit[1]
	EN_DRV_CLK_MASK_AHB_ENCRYPT,								///< bit[2]
	EN_DRV_CLK_MASK_AHB_DMA,									///< bit[3]
	EN_DRV_CLK_MASK_AHB_AGE,									///< bit[4]
	EN_DRV_CLK_MASK_AHB_AHB2AXI,								///< bit[5] 
	EN_DRV_CLK_MASK_AHB_IIC,									///< bit[6]
	EN_DRV_CLK_MASK_AHB_PWM0, 									///< bit[7]
	EN_DRV_CLK_MASK_AHB_PWM1, 									///< bit[8]
	EN_DRV_CLK_MASK_AHB_PWM2, 									///< bit[9]
	EN_DRV_CLK_MASK_AHB_TIMER0,									///< bit[10]
	EN_DRV_CLK_MASK_AHB_TIMER1,									///< bit[11]
	EN_DRV_CLK_MASK_AHB_TIMER2,									///< bit[12]
	EN_DRV_CLK_MASK_AHB_WDOG,									///< bit[13]
	EN_DRV_CLK_MASK_AHB_AXIDMA,									///< bit[14]
	EN_DRV_CLK_MASK_AHB_TCON,									///< bit[15]
	EN_DRV_CLK_MASK_AHB_TCONIR,									///< bit[16]
	EN_DRV_CLK_MASK_AHB_TCONLED,								///< bit[17]

	///< Partition C, AHB clock mask (0x08)
	EN_DRV_CLK_MASK_AHB_GPIO			= ((0x08 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AHB_UART0,									///< bit[1]
	EN_DRV_CLK_MASK_AHB_UART1,									///< bit[2]
	EN_DRV_CLK_MASK_AHB_DSP,									///< bit[3]

	///< Partition D, AHB clock mask (0x0C)
	EN_DRV_CLK_MASK_AHB_USB				= ((0x0C << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AHB_ATVFRONT,								///< bit[1]
	EN_DRV_CLK_MASK_AHB_DISP			= ((0x0C << 3) + 3),	///< bit[3]
	EN_DRV_CLK_MASK_AHB_TTX,									///< bit[4]
	EN_DRV_CLK_MASK_AHB_HDMI_AUD,								///< bit[5]
	EN_DRV_CLK_MASK_AHB_SPI,									///< bit[6]
	EN_DRV_CLK_MASK_AHB_SM,										///< bit[7]

	///< Partition E & F, AHB clock mask (0xC8)
	EN_DRV_CLK_MASK_AHB_OSD				= ((0xC8 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AHB_ATVBACK,								///< bit[1]
	EN_DRV_CLK_MASK_AHB_2DTO3D,									///< bit[2]
	EN_DRV_CLK_MASK_AHB_IMVQ_PCLK		= ((0xC8 << 3) + 8),	///< bit[8]
	EN_DRV_CLK_MASK_AHB_IMVQ_HCLK,								///< bit[9]

	///< Partition A, AXI clock mask (0x10)
	EN_DRV_CLK_MASK_AXI_AUD				= ((0x10 << 3) + 0), 	///< bit[0]
	EN_DRV_CLK_MASK_AXI_MAC, 									///< bit[1]
	EN_DRV_CLK_MASK_AXI_TVE, 									///< bit[2]
	EN_DRV_CLK_MASK_AXI_DEMOD,									///< bit[3]
	EN_DRV_CLK_MASK_AXI_LZD,									///< bit[4]
	EN_DRV_CLK_MASK_AXI_VIF,									///< bit[5]
	EN_DRV_CLK_MASK_AXI_AC_DATA,								///< bit[6]

	///< Partition B, AXI clock mask (0x14)
	EN_DRV_CLK_MASK_AXI_TS				= ((0x14 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AXI_ENCRYPT,								///< bit[1]
	EN_DRV_CLK_MASK_AXI_DMA,									///< bit[2]
	EN_DRV_CLK_MASK_AXI_AGE,									///< bit[3]
	EN_DRV_CLK_MASK_AXI_AHB2AXI,								///< bit[4]
	EN_DRV_CLK_MASK_AXI_AXIMDA,									///< bit[5]
	EN_DRV_CLK_MASK_AXI_TCON,									///< bit[6]

	///< Partition C, AXI clock mask (0x18)
	EN_DRV_CLK_MASK_AXI_DSP				= ((0x18 << 3) + 0),	///< bit[0]

	///< Partition D, AXI clock mask (0x1C)
	EN_DRV_CLK_MASK_AXI_USB				= ((0x1C << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AXI_ATVFRONT,								///< bit[1]
	EN_DRV_CLK_MASK_AXI_DISP			= ((0x1C << 3) + 3),	///< bit[3]
	EN_DRV_CLK_MASK_AXI_TTX,									///< bit[4]
	EN_DRV_CLK_MASK_AXI_HDMI_AUD,								///< bit[5]

	///< Partition E & F, AXI clock mask (0xCC)
	EN_DRV_CLK_MASK_AXI_OSD				= ((0xCC << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_AXI_IMVQ			= ((0xCC << 3) + 8),	///< bit[8]

	///< Partition A, engine clock mask (0x20)
	EN_DRV_CLK_MASK_CORE_MAC_25M		= ((0x20 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_CORE_MAC_50M, 								///< bit[1]
	EN_DRV_CLK_MASK_CORE_TVE_27M, 								///< bit[2]
	EN_DRV_CLK_MASK_CORE_DEMOD_C, 								///< bit[3]
	EN_DRV_CLK_MASK_CORE_DEMOD_F, 								///< bit[4]
	EN_DRV_CLK_MASK_CORE_AUD_SIF_27M,							///< bit[5]
	EN_DRV_CLK_MASK_CORE_AUD_SIF_108M,							///< bit[6]
	EN_DRV_CLK_MASK_CORE_SPDIF_IN,								///< bit[7]
	EN_DRV_CLK_MASK_CORE_DVIF_108M,								///< bit[8]

	///< Partition B, engine clock mask (0x24)
	EN_DRV_CLK_MASK_CORE_LEON			= ((0x24 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_CORE_LEON_INV,								///< bit[1]
	EN_DRV_CLK_MASK_CORE_TS0, 									///< bit[2]
	EN_DRV_CLK_MASK_CORE_TS1, 									///< bit[3]
	EN_DRV_CLK_MASK_CORE_TS_27M,								///< bit[4]
	EN_DRV_CLK_MASK_CORE_TCON_DISP,								///< bit[5]
	EN_DRV_CLK_MASK_CORE_TCON_LED_DISP,							///< bit[6]
	EN_DRV_CLK_MASK_CORE_TCON_OSC,								///< bit[7]
	EN_DRV_CLK_MASK_CORE_TCON_LED_SPI,							///< bit[8]

	///< Parition C, engine clock mask (0x28)
	EN_DRV_CLK_MASK_CORE_DSP			= ((0x28 << 3) + 0),	///< bit[0]

	///< Parition D, engine clock mask (0x2C)
	EN_DRV_CLK_MASK_CORE_AXI_PIXEL		= ((0x2C << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_CORE_ATV_VCLK,								///< bit[1]
	EN_DRV_CLK_MASK_CORE_ATV_I3,								///< bit[2]
	EN_DRV_CLK_MASK_CORE_ATV_I3_DISP,							///< bit[3]
	EN_DRV_CLK_MASK_CORE_ATV_VD2X,								///< bit[4]
	EN_DRV_CLK_MASK_CORE_ATV_MPLLCAP,							///< bit[5]
	EN_DRV_CLK_MASK_CORE_TVADC123_R, 							///< bit[6]
	EN_DRV_CLK_MASK_CORE_TVADC123_G, 							///< bit[7]
	EN_DRV_CLK_MASK_CORE_TVADC123_B, 							///< bit[8]
	EN_DRV_CLK_MASK_CORE_USBPHY_12M, 							///< bit[9]
	EN_DRV_CLK_MASK_CORE_USB0_30M, 								///< bit[10]
	EN_DRV_CLK_MASK_CORE_USB1_30M, 								///< bit[11]
	EN_DRV_CLK_MASK_CORE_ADC_BIST, 								///< bit[12]
	EN_DRV_CLK_MASK_CORE_ADC_BIST_TVADC0,						///< bit[13]
	EN_DRV_CLK_MASK_CORE_ADC_BIST_SIFADC,						///< bit[14]
	EN_DRV_CLK_MASK_CORE_HDMI_AUDIO_27M,						///< bit[15]

	///< Partition E & F, engine clock mask (0xD0)
	EN_DRV_CLK_MASK_CORE_2DTO3D			= ((0xD0 << 3) + 0),	///< bit[0]
	EN_DRV_CLK_MASK_CORE_IMVQ0, 								///< bit[1]
	EN_DRV_CLK_MASK_CORE_IMVQ1, 								///< bit[2]

	///< \n
	EN_DRV_CLK_MASK_MAX					= ((0xD0 << 3) + 31),	///< Maximum value of EN_DRV_CLK_MASK, don't add entry after this
} EN_DRV_CLK_MASK;
//@}

/**
 * @ingroup type
 *
 * @defgroup RST Emueration of clock reset
 * @{
 *
 * @var EN_DRV_CLK_RST
 *
 * The enumeration of clock reset
 *
 * @hideinitializer
 */
typedef enum _EN_DRV_CLK_RST
{
	EN_DRV_CLK_RST_MIN = 0,										///< Minimum value of EN_DRV_CLK_RST, don't add entry before this

	///< Parition A, core clock reset disable/enable (0x50/0x54)
	EN_DRV_CLK_RST_CORE_TVE	= EN_DRV_CLK_RST_MIN,	///< bit[0]
	EN_DRV_CLK_RST_CORE_DEMOD,						///< bit[1]
	EN_DRV_CLK_RST_CORE_TSIF,						///< bit[3]
	EN_DRV_CLK_RST_CORE_DVIF_PLLSC,					///< bit[4]
	EN_DRV_CLK_RST_CORE_DVIF_27M,					///< bit[5]
	EN_DRV_CLK_RST_CORE_SPDIF,						///< bit[6]

	///< Partition B, core clock reset disable/enable (0x58/0x5C)
	EN_DRV_CLK_RST_CORE_TS,							///< bit[0]
	EN_DRV_CLK_RST_CORE_TS_PARSER1,					///< bit[1]
	EN_DRV_CLK_RST_CORE_TS_PARSER2,					///< bit[2]
	EN_DRV_CLK_RST_CORE_ENCRYPT,					///< bit[3]
	EN_DRV_CLK_RST_CORE_TCON,						///< bit[4]
	EN_DRV_CLK_RST_CORE_TCON_ASYC_WCLK,				///< bit[5]
	EN_DRV_CLK_RST_CORE_TCON_DISP,					///< bit[6]
	EN_DRV_CLK_RST_CORE_TCON_DP,					///< bit[7]
	EN_DRV_CLK_RST_CORE_TCON_SPI,					///< bit[8]
	EN_DRV_CLK_RST_CORE_TCON_DISP_LED,				///< bit[9]

	///< Partition C, core clock reset disable/enable (0x60/0x64)
	EN_DRV_CLK_RST_CORE_MIPS,						///< bit[0]
	EN_DRV_CLK_RST_CORE_MIPS_MCLK,					///< bit[1]
	EN_DRV_CLK_RST_CORE_MAU,						///< bit[2]
	EN_DRV_CLK_RST_CORE_DSP,						///< bit[11]

	///< Partition D, core clock reset disable/enable (0x68/0x6C)
	EN_DRV_CLK_RST_CORE_DISP,						///< bit[1]
	EN_DRV_CLK_RST_CORE_HDMIPHY,					///< bit[2]
	EN_DRV_CLK_RST_CORE_HDMI_AUDIO,					///< bit[3]
	EN_DRV_CLK_RST_CORE_USB0,						///< bit[4]
	EN_DRV_CLK_RST_CORE_USB1,						///< bit[5]
	EN_DRV_CLK_RST_CORE_TVADC_BIST,					///< bit[6]
	EN_DRV_CLK_RST_CORE_SIFADC_BIST,				///< bit[7]

	///< Partition E & F, core clock reset disable/enable (0xD4/0xD8)bit
	EN_DRV_CLK_RST_CORE_2DTO3D,						///< bit[0]
	EN_DRV_CLK_RST_CORE_OSD,						///< bit[1]
	EN_DRV_CLK_RST_CORE_IMVQ_H264,					///< bit[8]
	EN_DRV_CLK_RST_CORE_IMVQ_MPEG,					///< bit[9]

	///< Partition A, AXI clock reset disable/enable (0x70/0x74)
	EN_DRV_CLK_RST_AXI_MAC,							///< bit[0]
	EN_DRV_CLK_RST_AXI_TVE,							///< bit[1]
	EN_DRV_CLK_RST_AXI_DVIF,						///< bit[2]
	EN_DRV_CLK_RST_AXI_AUDIO,						///< bit[3]
	EN_DRV_CLK_RST_AXI_AUDIO_LZD,					///< bit[4]
	EN_DRV_CLK_RST_AXI_AUDIO_AC_DATA,				///< bit[5]
	
	///< Partition B, AXI clock reset disable/enable (0x78/0x7C)
	EN_DRV_CLK_RST_AXI_TS,							///< bit[0]
	EN_DRV_CLK_RST_AXI_AGE,							///< bit[1]
	EN_DRV_CLK_RST_AXI_AHB2AXI,						///< bit[2]
	EN_DRV_CLK_RST_AXI_DMA0,						///< bit[3]
	EN_DRV_CLK_RST_AXI_ENCRYPT,						///< bit[4]
	EN_DRV_CLK_RST_AXI_TCON,						///< bit[5]
	EN_DRV_CLK_RST_AXI_DMA1,						///< bit[6]

	///< Partition C, AXI clock reset disable/enable (0x80/0x84)
	EN_DRV_CLK_RST_AXI_MAU,							///< bit[0]
	EN_DRV_CLK_RST_AXI_DSP,							///< bit[4]

	///< Partition D, AXI clock reset disable/enable (0x88/0x8C)
	EN_DRV_CLK_RST_AXI_ATVFRONT,					///< bit[0]
	EN_DRV_CLK_RST_AXI_TTX,							///< bit[2]
	EN_DRV_CLK_RST_AXI_DISP,						///< bit[3]
	EN_DRV_CLK_RST_AXI_HDMI_AUDIO,					///< bit[4]
	EN_DRV_CLK_RST_AXI_USB0,						///< bit[5]
	EN_DRV_CLK_RST_AXI_USB1,						///< bit[6]

	///< Partition E & F, AXI clock reset disable/enable (0xDC/0xE0)
	EN_DRV_CLK_RST_AXI_OSD,							///< bit[0]
	EN_DRV_CLK_RST_AXI_IMVQ,						///< bit[8]

	///< Parition A, AHB clock reset disable/enable (0x90/0x94)
	EN_DRV_CLK_RST_AHB_MAC,							///< bit[0]
	EN_DRV_CLK_RST_AHB_TVE,							///< bit[1]
	EN_DRV_CLK_RST_AHB_AUDIO,						///< bit[2]
	EN_DRV_CLK_RST_AHB_AUDIO_CK0,					///< bit[3]
	EN_DRV_CLK_RST_AHB_AUDIO_CK1,					///< bit[4]
	EN_DRV_CLK_RST_AHB_DVIF,						///< bit[5]
	EN_DRV_CLK_RST_AHB_LZD,							///< bit[6]
	EN_DRV_CLK_RST_AHB_AC_DATA,						///< bit[7]

	///< Partition B, AHB clock reset disable/enable (0x98/0x9C)
	EN_DRV_CLK_RST_AHB_TS,							///< bit[0]
	EN_DRV_CLK_RST_AHB_CI,							///< bit[1]
	EN_DRV_CLK_RST_AHB_AGE,							///< bit[2]
	EN_DRV_CLK_RST_AHB_AHB2AXI,						///< bit[3]
	EN_DRV_CLK_RST_AHB_DMA0,						///< bit[4]
	EN_DRV_CLK_RST_AHB_ENCRYPT,						///< bit[5]
	EN_DRV_CLK_RST_AHB_TIMER0,						///< bit[6]
	EN_DRV_CLK_RST_AHB_TIMER1,						///< bit[7]
	EN_DRV_CLK_RST_AHB_PWM0,						///< bit[8]
	EN_DRV_CLK_RST_AHB_PWM1,						///< bit[9]
	EN_DRV_CLK_RST_AHB_PWM2,						///< bit[10]
	EN_DRV_CLK_RST_AHB_HOUT,						///< bit[11]
	EN_DRV_CLK_RST_AHB_TCON,						///< bit[12]
	EN_DRV_CLK_RST_AHB_TCON_IR,						///< bit[13]
	EN_DRV_CLK_RST_AHB_TCON_LED,					///< bit[14]
	EN_DRV_CLK_RST_AHB_MINI_LVDS,					///< bit[15]
	EN_DRV_CLK_RST_AHB_DMA1,						///< bit[16]

	///< Partition C, AHB clock reset disable/enable (0xA0/0xA4)
	EN_DRV_CLK_RST_AHB_DDR2PHY,						///< bit[3]

	///< Partition D, AHB clock reset disable/enable (0xA8/0xAC)
	EN_DRV_CLK_RST_AHB_ATVFRONT,					///< bit[0]
	EN_DRV_CLK_RST_AHB_TTX,							///< bit[1]
	EN_DRV_CLK_RST_AHB_DISP, 						///< bit[2]
	EN_DRV_CLK_RST_AHB_USB0, 						///< bit[3]
	EN_DRV_CLK_RST_AHB_USB1, 						///< bit[4]

	///< Partition E & F, AHB clock reset disable/enable (0xE4/0xE8)
	EN_DRV_CLK_RST_AHB_OSD,							///< bit[0]
	EN_DRV_CLK_RST_AHB_2DTO3D,						///< bit[1]
	EN_DRV_CLK_RST_AHB_IMVQ0,						///< bit[8]
	EN_DRV_CLK_RST_AHB_IMVQ1,						///< bit[9]

	///< \n
	EN_DRV_CLK_RST_MAX,								///< Maximum value of EN_KER_CLK_RST, don't add entry after this
} EN_DRV_CLK_RST;
//@}

/**
 * @defgroup funcs Function prototype
 * @{
 */

/**
 * @fn u32 clk_down_flow_sem(void)
 *
 * @brief  Get clock gen flow semaphore
 *
 * @code
 * down_trylock(&clk_flow_sem);
 * @endcode
 * 
 * @return Returns 0 if the mutex has been acquired successfully or 1 if it it cannot be acquired.
 *
 */
u32 clk_down_flow_sem(void);

/**
 * @fn void clk_up_flow_sem(void)
 *
 * @brief  Release clock gen flow semaphore
 *
 * @code
 * up(&clk_flow_sem);
 * @endcode
 *
 */
void clk_up_flow_sem(void);

/**
 * @fn u8 clk_set_mpll_byte_reg(bool b8PageB, u8 u8Offset, u8 u8Value)
 *
 * @brief  Set MPLL register by byte unit
 * 
 * @param[in]  b8PageB	TRUE: page-B, FALSE: page-0
 * @param[in]  u8Offset	register offset
 * @param[in]  u8Value	new register value
 *
 * @return the new register value
 *
 */
u8 clk_set_mpll_byte_reg(bool b8PageB, u8 u8Offset, u8 u8Value);

/**
 * @fn u8 clk_get_mpll_byte_reg(bool b8PageB, u8 u8Offset)
 *
 * @brief  Get MPLL register by byte unit
 * 
 * @param[in]  b8PageB	TRUE: page-B, FALSE: page-0
 * @param[in]  u8Offset	register offset
 *
 * @return the current register value
 *
 */
u8 clk_get_mpll_byte_reg(bool b8PageB, u8 u8Offset);

/**
 * @fn u32 clk_set_mpll_word_reg(bool b8PageB, u8 u8Offset, u32 u32Num, u32 u32Value)
 *
 * @brief  Set MPLL register by word(4bytes) unit
 * 
 * @param[in]  b8PageB	TRUE: page-B, FALSE: page-0
 * @param[in]  u8Offset	register offset
 * @param[in]  u32Num  	number of register, maximun is 4.
 * @param[in]  u32Value	new register value
 *
 * @return the new register value
 *
 */
u32 clk_set_mpll_word_reg(bool b8PageB, u8 u8Offset, u32 u32Num, u32 u32Value);

/**
 * @fn u32 clk_get_mpll_word_reg(bool b8PageB, u8 u8Offset, u32 u32Num)
 *
 * @brief  Get MPLL register by word(4bytes) unit
 * 
 * @param[in]  b8PageB	TRUE: page-B, FALSE: page-0
 * @param[in]  u8Offset	Register offset
 * @param[in]  u32Num	Number of register, maximun is 4.
 *
 * @return The current register value
 *
 */
u32 clk_get_mpll_word_reg(bool b8PageB, u8 u8Offset, u32 u32Num);

/**
 * @fn void clk_set_clock_source(EN_DRV_CLK_SRC enSrc, u32 u32Src)
 *
 * @brief  Set clock source for specific top or engine
 * 
 * @param[in]  enSrc	Indicate the clock source which will be changed.
 * @param[in]  u32Src	New clock source value
 *
 */
void clk_set_clock_source(EN_DRV_CLK_SRC enSrc, u32 u32Src);

/**
 * @fn void clk_get_clock_source(EN_DRV_CLK_SRC enSrc)
 *
 * @brief  Get clock source for specific top or engine
 * 
 * @param[in]  enSrc	Indicate the top or engine which will be changed.
 *
 * @return Clock source
 *
 */
u32 clk_get_clock_source(EN_DRV_CLK_SRC enSrc);

/**
 * @fn void clk_update(EN_DRV_CLK_MPLL_TYPE enType, UN_DRV_CLK_SETTING stSetting)
 *
 * @brief Update MPLL
 *
 * @param[in] enType	MPLL which will be updated
 * @param[in] stSetting Information for MPLL update	
 *
 */
void clk_update(EN_DRV_CLK_MPLL_TYPE enType, UN_DRV_CLK_SETTING stSetting);

/**
 * @fn u32 clk_get_mpll(EN_DRV_CLK_MPLL_TYPE enType)
 *
 * @brief Get MPLL (Hz)
 *
 * @param[in] enType	MPLL which want to get 
 *
 * @return Value of MPLL (Hz)
 *
 */
u32 clk_get_mpll(EN_DRV_CLK_MPLL_TYPE enType);

/**
 * @fn void clk_set_clock_inv(EN_DRV_CLK_INV enInv, bool b8EnInv)
 *
 * @brief Clock inverse enable/disable
 *
 * @param[in] enInv	Clock mask which will be enable or disable
 * @param[in] b8EnInv	TRUE: clock inverse enable, FALSE clock inversse disable
 *
 */
void clk_set_clock_inv(EN_DRV_CLK_INV enInv, bool b8EnInv);

/**
 * @fn void clk_set_clock_mask(EN_DRV_CLK_MASK enMask, bool b8EnMask)
 *
 * @brief Clock mask enable/disable
 *
 * @param[in] enMask	Clock mask which will be enable or disable
 * @param[in] b8EnMask	TRUE: clock mask enable, FALSE clock mask disable
 *
 */
void clk_set_clock_mask(EN_DRV_CLK_MASK enMask, bool b8EnMask);

/**
 * @fn void clk_set_clock_reset(EN_DRV_CLK_RST enRst, bool b8EnRst)
 *
 * @brief Clock reset enable/disable
 *
 * @param[in] enRst		Clock reset which will be enable or disable
 * @param[in] b8EnRst	TRUE: enable clock reset, FALSE: disable clock reset
 *
 */
void clk_set_clock_reset(EN_DRV_CLK_RST enRst, bool b8EnRst);

/**
 * @fn void clk_reset_clock(EN_DRV_CLK_RST enRst)
 *
 * @brief Reset clock (enable reset (if 1)  and then disable reset)
 *
 * @param[in] enRst	Clock which will be reset
 *
 */
void clk_reset_clock(EN_DRV_CLK_RST enRst);

/**
 * @fn u32 clk_get_ahb_clock(void)
 *
 * @brief Get value of AHB clock (Hz)
 *
 * @return Value of AHB clock
 */
u32 clk_get_ahb_clk(void);
//@}

#ifdef __cplusplus
extern }
#endif

#endif

