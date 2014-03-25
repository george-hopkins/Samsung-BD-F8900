/*
 * linux/arch/arm/mach-sdp/sdp1105_gpio.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : seungjun.heo@samsung.com 	06/000000000/2011
 *
 */

#include <plat/sdp_gpio.h>

struct _sdp_gpio_pull sdp_gpio_pull_list[] =
{
	{ 0, 0, 0x04, 0},//GPIO00
	{ 0, 1, 0x04, 2},
	{ 0, 2, 0x04, 4},
	{ 0, 3, 0x04, 6},
	{ 0, 4, 0x04, 8},
	{ 0, 5, 0x04, 10},
	{ 0, 6, 0x04, 12},
	{ 0, 7, 0x04, 14},//GPIO07
	
	{ 1, 0, 0x00, 12},//GPIO1
	{ 1, 1, 0x00, 14},
	{ 1, 2, 0x00, 16},
	{ 1, 3, 0x04, 18},//i_UART0_RX 
	{ 1, 4, 0x04, 20},//i_UART0_nCTS
	{ 1, 5, 0x04, 26},//i_UART1_RX
	{ 1, 6, 0x04, 28},//i_UART2_TX
	{ 1, 7, 0x04, 30},//i_UART2_rX
	
	{ 2, 0, 0x28, 0},//i_RMI_CEn
	{ 2, 1, 0x28, 2},//i_RMI_SCK
	{ 2, 2, 0x28, 4},//o_RMI_READY
	{ 2, 3, 0x28, 6},//o_RMI_RSCK
	{ 2, 4, 0x28, 8},//b_RMI_SIO0
	{ 2, 5, 0x28, 10},//b_RMI_SIO1
	{ 2, 6, 0x28, 12},//b_RMI_SIO2
	{ 2 ,7, 0x28, 14},//i_SPDIF_RX

	{ 3, 0, 0x20, 18},//o_BLC_PWM
	{ 3, 1, 0x08, 6 },//o_SPI_NSSI
	{ 3, 2, 0x10, 16},//i_NAND_FLASH

	{ 4, 0, 0x28, 16},//b_RMI_SIO4
	{ 4, 1, 0x28, 18},//b_RMI_SIO5
	{ 4, 2, 0x28, 20},//b_RMI_SIO6
	{ 4, 3, 0x28, 22},//b_RMI_SIO7
	{ 4, 4, 0x00, 28},//b_I2C2_CLK
	{ 4, 5, 0x00, 30},//b_I2C2_DATA
	{ 4, 6, 0x08, 2},//i_SPI_SIO 	
	{ 4, 7, 0x08, 26 },//i_SPDIF_RX
	
	{ 5, 0, 0x0C, 22},//b_I2S_RX0_MCLK
	{ 5, 1, 0x0C, 24},//b_I2S_RX0_LRCLK
	{ 5, 2, 0x0C, 26},//b_I2S_RX0_BCLK 
	{ 5, 3, 0x0C, 28},//i_I2S_RX0_SDI
	{ 5, 4, 0x10, 0},//b_I2S_RX1_MCLK
	{ 5, 5, 0x10, 2},//b_I2S_RX1_LRCLK
	{ 5, 6, 0x10, 4},//b_I2S_RX1_BCLK
	{ 5, 7, 0x10, 6},//i_I2S_RX1_SDI0
	
	{ 6, 0, 0x10, 8},//i_I2S_RX1_SDI1
	{ 6, 1, 0x10, 10},//i_I2S_RX1_SDI2
	{ 6, 2, 0x10, 12},//i_I2S_RX1_SDI3
	{ 6, 3, 0x1C, 4},//b_GPIO_USB0
	{ 6, 4, 0x1C, 6},//b_GPIO_USB1
	{ 6, 5, 0x1C, 10},//i_MAC_TX_CLK
	{ 6, 6, 0x1C, 16},//o_MAC_TXD2
	{ 6, 7, 0x1C, 18},//o_MAC_TXD3

	{ 7, 0, 0x1C, 26 },//o_MAC_TX_ER
	{ 7, 1, 0x20, 2 },//i_MAC_RXD2
	{ 7, 2, 0x20, 4 },//i_MAC_RXD3
	{ 7, 3, 0x20, 6},//i_MAC_RX_DV
	{ 7, 4, 0x20, 10},//i_MAC_COL ,
	{ 7, 5, 0x24, 0},//o_SDCARD_CLK 
	{ 7, 6, 0x24, 2 },//o_SDCARD_CMD 
	{ 7, 7, 0x24, 4 },//b_SDCARD_DATA0

	{ 8, 0, 0x24, 6 },//b_SDCARD_DATA1
	{ 8, 1, 0x24, 8 },//b_SDCARD_DATA2
	{ 8, 2, 0x24, 10 },//b_SDCARD_DATA3
	{ 8, 3, 0x24, 12},//b_SDCARD_DATA4
	{ 8, 4, 0x24, 14 },//b_SDCARD_DATA5
	{ 8, 5, 0x24, 16},//b_SDCARD_DATA6
	{ 8, 6, 0x24, 18},//b_SDCARD_DATA7
	{ 8, 7, 0x24, 20},//i_SDCARD_WP
	
	{ 9, 0, 0x24, 22},//i_SDCARD_DET_N


	{ 9, 3, 0x14, 0 },//b_HD_DE
	{ 9, 4, 0x20, 12},//i_MAC_CRS
	{ 9, 5, 0x20, 8 },//i_MAC_RX_ER
		
	{ 9, 7, 0x04, 22},//i_MAC_RX_ER
};

int sdp_gpio_pull_size = sizeof(sdp_gpio_pull_list) / sizeof(struct _sdp_gpio_pull);

