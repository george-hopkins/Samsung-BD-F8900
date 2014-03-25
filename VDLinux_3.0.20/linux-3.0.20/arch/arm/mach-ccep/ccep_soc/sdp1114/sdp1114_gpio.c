/*
 * linux/arch/arm/mach-sdp/sdp1114_gpio.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : seungjun.heo@samsung.com 	07/06/2011
 *
 */

#include <plat/sdp_gpio.h>

struct _sdp_gpio_pull sdp_gpio_pull_list[] =
{
	{ 0, 0, 0x04, 24},//GPIO0
	{ 0, 1, 0x04, 26},
	{ 0, 2, 0x04, 28},
	{ 0, 3, 0x04, 30},
	{ 0, 4, 0x08, 0},
	{ 0, 5, 0x08, 2},
	{ 0, 6, 0x08, 4},
	{ 0, 7, 0x08, 6},//GPIO7
	
	{ 1, 0, 0x08, 8},
	{ 1, 1, 0x08, 10},
	{ 1, 2, 0x08, 12},
	{ 1, 3, 0x08, 14},
	{ 1, 4, 0x08, 16},
	{ 1, 5, 0x08, 18},
	{ 1, 6, 0x08, 20},
	{ 1, 7, 0x08, 22},//GPIO15
	
	{ 2, 0, 0x08, 24 },//MSPI_CD
	{ 2, 1, 0x08, 26 },//MSPI_MDET
	{ 2, 2, 0x08, 28 },//MSPI_RESET
	{ 2, 3, 0x00, 10},//EXTINT0
	{ 2, 4, 0x00, 12 },
	{ 2, 5, 0x00, 14 },
	{ 2, 6, 0x00, 16 },
	{ 2, 7, 0x00, 18 },//EXTINT4
	
	{ 3, 0, 0x00, 20 },//UART0_TX 
	{ 3, 1, 0x00, 22 },
	{ 3, 2, 0x00, 24},
	{ 3, 3, 0x00, 26},
	{ 3, 4, 0x00, 28},
	{ 3, 5, 0x00, 30},//UART2_RX
	{ 3, 6, 0x04, 0},//UART3_TX
	{ 3, 7, 0x04, 2 },
//------	
	{ 4, 0, 0x04, 4 },//I2C0_CLK
	{ 4, 1, 0x04, 6 },
	{ 4, 2, 0x04, 8 },
	{ 4, 3, 0x04, 10 },
	{ 4, 4, 0x04, 12},
	{ 4, 5, 0x04, 14},
	{ 4, 6, 0x04, 16},
	{ 4, 7, 0x04, 18 },//I2C3_DATA
	
	{ 5, 0, 0x04, 20},//I2C4_CLK
	{ 5, 1, 0x04, 22},//I2C4_DATA
	{ 5, 2, 0x08, 30},//CI_RESET
	{ 5, 3, 0x0C, 0},//NREG
	{ 5, 4, 0x0C, 2},//NIORD 
	{ 5, 5, 0x0C, 4},//NIOWR 
	{ 5, 6, 0x0C, 6},//SCRESET 
	{ 5, 7, 0x0C, 8},//SCVCCN
	
	{ 6, 0, 0x0C, 10},//SCCLK
	{ 6, 1, 0x0C, 12},//SCDETECTN 
	{ 6, 2, 0x0C, 14},//SCDATA 
	{ 6, 3, 0x0C, 16},//MOVI0_CLK
	{ 6, 4, 0x0C, 18},//MOVI0_CMD 
	{ 6, 5, 0x0C, 20},//MOVI0_DATA0 
	{ 6, 6, 0x0C, 22},//MOVI0_DATA1 
	{ 6, 7, 0x0C, 24},//MOVI0_DATA2
	
	{ 7, 0, 0x0C, 26 },//MOVI0_DATA3
	{ 7, 1, 0x0C, 28 },//MOVI0_DATA4
	{ 7, 2, 0x0C, 30 },//MOVI0_DATA5
	{ 7, 3, 0x10, 0},//MOVI0_DATA6
	{ 7, 4, 0x10, 2 },//MOVI0_DATA7 
	{ 7, 5, 0x10, 4 },//MOVI1_CLK 
	{ 7, 6, 0x10, 6 },//MOVI1_CMD 
	{ 7, 7, 0x10, 8 },//MOVI1_DATA0
	
	{ 8, 0, 0x10, 10 },//MOVI1_DATA1
	{ 8, 1, 0x10, 12 },//MOVI1_DATA2
	{ 8, 2, 0x10, 14 },//MOVI1_DATA3
	{ 8, 3, 0x10, 16},//MOVI1_DATA4
	{ 8, 4, 0x10, 18 },//MOVI1_DATA5
	{ 8, 5, 0x10, 20},//MOVI1_DATA6
	{ 8, 6, 0x10, 22},//MOVI1_DATA7
	{ 8, 7, 0x10, 24 },//MOVI_HW_RESET
	
	{ 9, 0, 0x10, 26 },//MOVI_SAMPLE_CLK
	{ 9, 1, 0x10, 28 },
	{ 9, 2, 0x10, 30 },//MOVI_CLK_SEL
	{ 9, 3, 0x14, 0 },//MOVI_BOOT_EN
	{ 9, 4, 0x14, 2 },
	{ 9, 5, 0x14, 4 },
	{ 9, 6, 0x14, 6 },
	{ 9, 7, 0x14, 8 },//EMAC_PHY_TXCLK
	
	{ 10, 0, 0x14, 10},//EMAC_PHY_TXEN
	{ 10, 1, 0x14, 12},
	{ 10, 2, 0x14, 14},
	{ 10, 3, 0x14, 16},
	{ 10, 4, 0x14, 18},
	{ 10, 5, 0x14, 20},
	{ 10, 6, 0x14, 22},
	{ 10, 7, 0x14, 24},//EMAC_PHY_RXD0
	
	{ 11, 0, 0x14, 26 },//EMAC_PHY_RXD1
	{ 11, 1, 0x14, 28 },
	{ 11, 2, 0x14, 30 },//EMAC_PHY_RXD3
	{ 11, 3, 0x18, 0},
	{ 11, 4, 0x18, 2},
	{ 11, 5, 0x18, 4},
	{ 11, 6, 0x18, 6},
	{ 11, 7, 0x18, 8},//I2EPWM3

	{ 12, 0, 0x18, 10},//I2EPWM4
	{ 12, 1, 0x18, 12},//IRRX
	{ 12, 2, 0x1C, 2},//TSI1_CLK
	{ 12, 3, 0x1C, 4},
	{ 12, 4, 0x1C, 6},
	{ 12, 5, 0x1C, 8},
	{ 12, 6, 0x1C, 10},
	{ 12, 7, 0x1C, 12},//TSI1_DATA3

	{ 13, 0, 0x1C, 14 },//TSI1_DATA4 
	{ 13, 1, 0x1C, 16 },
	{ 13, 2, 0x1C, 18},
	{ 13, 3, 0x1C, 20},
	{ 13, 4, 0x1C, 22},
	{ 13, 5, 0x1C, 24},
	{ 13, 6, 0x1C, 26},
	{ 13, 7, 0x1C, 28},//TSI2_SYNC

	{ 14, 0, 0x1C, 30},//TSI2_DATA
	{ 14, 1, 0x20, 26 },//I2S_TX2_MCLK
	{ 14, 2, 0x20, 28 },
	{ 14, 3, 0x20, 30 },//I2S_TX2_BCLK
	{ 14, 4, 0x24, 0},//I2S_TX2_SDO
	{ 14, 5, 0x24, 2},
	{ 14, 6, 0x24, 4},
	{ 14, 7, 0x24, 6 },//I2S_RX0_BCLK

	{ 15, 0, 0x24, 18},//SM_WAIT
	{ 15, 1, 0x24, 20},//CANCEL_WAIT
	{ 15, 2, 0x24, 28},//CS0
	{ 15, 3, 0x24, 30},
	{ 15, 4, 0x28, 0},
	{ 15, 5, 0x28, 2},//CS3
	{ 15, 6, 0x40, 4},//USB0_VBUSVALID 
	{ 15, 7, 0x40, 6},//USB1_VBUSVALID 
};

int sdp_gpio_pull_size = sizeof(sdp_gpio_pull_list) / sizeof(struct _sdp_gpio_pull);

