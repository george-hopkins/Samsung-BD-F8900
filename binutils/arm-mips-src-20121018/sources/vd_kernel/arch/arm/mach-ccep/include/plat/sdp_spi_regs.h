/*********************************************************************************************
 *
 *	sdp_spi.h (Samsung Soc SPI master device regster) 
 *
 *	author : drain.lee@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 

 ********************************************************************************************/

#ifndef __SDP_SPI_REG_H
#define __SDP_SPI_REG_H


#if 0/* pad init code move to mach-ccep/sdp****.c */
//#if defined(CONFIG_ARCH_SDP1001)	//for GenoaS


#if defined(CONFIG_ARCH_SDP1002)	//for GenoaP
//MSPI REG TX_CTRL for Select SPI Module
#define REG_MSPI_TX_CTRL	0x30090268// set 0x1000 for sel SPI
#define MSPI_TX_CTRL_SELSPI (0x01 << 12)
//PAD Control register for Select SPI Module
#define REG_PAD_CTRL	0x30090DD4
#define PAD_CTRL_SELSPI	(0x01 << 15)

#elif defined(CONFIG_ARCH_SDP1004)	//for Firenze
//if used o_SPI_nSSI then 656_IN_SEL and AUDIO_JTAG_SEL is not select.
#define REG_SUB_FUNC_SEL	0x30090CD0//pad control main/sub function selection register
#define SUB_FUNC_SEL_656_IN_SEL (0x01 << 11)
#define SUB_FUNC_SEL_AUDIO_JTAG_SEL (0x01 << 8)

#elif defined(CONFIG_ARCH_SDP1103)	//for santos
#define REG_PAD_CTRL	0x30090C50
#define PAD_CTRL_SELSPI	(0x01 << 20)
#define REG_MSPI_TX_CTRL	0x30090268// set 0x1000 for sel SPI
#define MSPI_TX_CTRL_SELSPI (0x01 << 12)

#elif defined(CONFIG_ARCH_SDP1105)	//for Victoria

#elif defined(CONFIG_ARCH_SDP1106)	//for flamengo
#define REG_PAD_CTRL	0x30090C50
#define PAD_CTRL_SELSPI	(0x01 << 20)
#define REG_MSPI_TX_CTRL	0x30090268// set 0x1000 for sel SPI
#define MSPI_TX_CTRL_SELSPI (0x01 << 12)

#else
#error "Platform is not defined!!!"

#endif

#endif/* pad init code move to mach-ccep/sdp****.c */

/** Registers **/
#define SSPCR0			0x0000
#define SSPCR0_MODE_SHIFT	6
#define SSPCR0_SCR_SHIFT	8
#define SSPCR0_SPH		BIT(7)
#define SSPCR0_SPO		BIT(6)
#define SSPCR0_FRF_SHIFT	4  //00:spi, 01:it ss, 10:Microwire
#define SSPCR0_DSS_SHIFT	0  //0011b ~ 1111b(4~16bit)

#define SSPCR1			0x0004
#define SSPCR1_RIE		BIT(0)
#define SSPCR1_TIE		BIT(1)
#define SSPCR1_RORIE		BIT(2)
#define SSPCR1_LBM		BIT(3)
#define SSPCR1_SSE		BIT(4)
//#define SSPCR1_MS		BIT(5)
//#define SSPCR1_SOD		BIT(6)

#define SSPDR			0x0008

#define SSPSR			0x000c
#define SSPSR_TFE		BIT(0)
#define SSPSR_TNF		BIT(1)
#define SSPSR_RNE		BIT(2)
#define SSPSR_RFF		BIT(3)
#define SSPSR_BSY		BIT(4)

#define SSPCPSR			0x0010 // 2~254

#define SSPIIR			0x0014
#define SSPIIR_RIS		BIT(0)
#define SSPIIR_TIS		BIT(1)
#define SSPIIR_RORIS		BIT(2)
#define SSPICR			SSPIIR


#endif //__SDP_SPI_REG_H