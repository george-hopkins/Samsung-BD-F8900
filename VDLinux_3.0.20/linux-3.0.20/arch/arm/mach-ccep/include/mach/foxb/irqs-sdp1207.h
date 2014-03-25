/*
 * irqs-sdp1207.h
 *
 * Copyright (c) 2012 Samsung Electronics
 * Tukho Kim <tukho.kim@samsung.com>
 *
 */

#ifndef _SDP1207_IRQS_H_
#define _SDP1207_IRQS_H_

#ifdef CONFIG_ARM_GIC

#define IRQ_PPI(x)	((x) + 16)
#define IRQ_LOCALTIMER	IRQ_PPI(13)

#define IRQ_SPI(x)	((x) + 32)

#define NR_IRQS			IRQ_SPI(224)	

#define IRQ_TSD			IRQ_SPI(0)
#define IRQ_SE			IRQ_SPI(1)
#define IRQ_AIO			IRQ_SPI(2)
#define IRQ_PNG			IRQ_SPI(3)
#define IRQ_GZIP		IRQ_SPI(4)
#define IRQ_JPEG		IRQ_SPI(5)
#define IRQ_MFC			IRQ_SPI(6)
#define IRQ_AE			IRQ_SPI(7)
#define IRQ_HDMI		IRQ_SPI(8)
#define IRQ_OSDP        	IRQ_SPI(9)
#define IRQ_GA2D        	IRQ_SPI(10)
#define IRQ_GPU_GPMMU		IRQ_SPI(11)
#define IRQ_GPU_GP		IRQ_SPI(12)
#define IRQ_GPU_PPMMU		IRQ_SPI(13)
#define IRQ_GPU_PP		IRQ_SPI(14)
#define IRQ_CSSYS_CPU0		IRQ_SPI(15)
#define IRQ_CSSYS_CPU1		IRQ_SPI(16)
#define IRQ_XMIF		IRQ_SPI(17)
#define IRQ_DISP		IRQ_SPI(18)
#define IRQ_XROAD		IRQ_SPI(19)
#define IRQ_EHCI0		IRQ_SPI(20)
#define IRQ_OHCI0		IRQ_SPI(21)
#define IRQ_EHCI1		IRQ_SPI(22)
#define IRQ_OHCI1		IRQ_SPI(23)
#define IRQ_GMAC		IRQ_SPI(24)
#define IRQ_SATA0		IRQ_SPI(25)
#define IRQ_SATA1		IRQ_SPI(26)
#define IRQ_SDMMC		IRQ_SPI(27)
#define IRQ_SSP			IRQ_SPI(28)
#define IRQ_GDMA		IRQ_SPI(29)
#define IRQ_AMSDMA		IRQ_SPI(30)
#define IRQ_SMCDMA		IRQ_SPI(31)
#define IRQ_TIMER		IRQ_SPI(32)
#define IRQ_TIMER0		IRQ_SPI(32)                                 
#define IRQ_TIMER1		IRQ_SPI(33)                                 
#define IRQ_TIMER2		IRQ_SPI(34)                                 
#define IRQ_TIMER3		IRQ_SPI(35)
#define IRQ_UART		IRQ_SPI(36)
#define IRQ_UART0		IRQ_SPI(36)
#define IRQ_UART1		IRQ_SPI(37)
#define IRQ_UART2		IRQ_SPI(38)
#define IRQ_UART3		IRQ_SPI(39)
#define IRQ_IIC0		IRQ_SPI(40)
#define IRQ_IIC1		IRQ_SPI(41)
#define IRQ_IIC2		IRQ_SPI(42)
#define IRQ_IIC3		IRQ_SPI(43)
#define IRQ_VBUSVALID1		IRQ_SPI(44)
#define IRQ_VBUSVALID0		IRQ_SPI(45)
#define IRQ_MICOM0		IRQ_SPI(46)
#define IRQ_MICOM1		IRQ_SPI(47)
#define IRQ_EXT0		IRQ_SPI(48)
#define IRQ_EXT1		IRQ_SPI(49)
#define IRQ_EXT2		IRQ_SPI(50)
#define IRQ_EXT3		IRQ_SPI(51)
#define IRQ_EXT4		IRQ_SPI(52)
#define IRQ_EXT5		IRQ_SPI(53)
#define IRQ_EXT6		IRQ_SPI(54)
#define IRQ_EXT7		IRQ_SPI(55)
#define IRQ_UDMA_RX		IRQ_SPI(56)
#define IRQ_UDMA_TX		IRQ_SPI(57)
#define IRQ_RTC			IRQ_SPI(58)
#define IRQ_SPI0				IRQ_SPI(59)
#define IRQ_SCI			IRQ_SPI(60)
#define IRQ_PMU0		IRQ_SPI(61)
#define IRQ_PMU1		IRQ_SPI(62)
#define IRQ_nIRQ0			IRQ_SPI(63)
#define IRQ_nIRQ1			IRQ_SPI(64)
#define IRQ_nFIQ0			IRQ_SPI(65)
#define IRQ_nFIQ1			IRQ_SPI(66)

#define IRQ_TDSP	    	IRQ_AE
#define IRQ_GPPLANE		IRQ_OSDP
#define IRQ_2DGA        	IRQ_GA2D

#else

#define NR_IRQS			32

#define IRQ_TSD     	    	0
#define IRQ_AIO         	1
#define IRQ_BDGA		2
#define IRQ_TIMER       	3
#define IRQ_SE			5
#define IRQ_AE         	   	6    
#define IRQ_HDMI         	7
#define IRQ_SATA           	8
#define IRQ_SATA1           	9
#define IRQ_RMI             	10
#define IRQ_DISP       		12
#define IRQ_USB_EHCI0   	13 
#define IRQ_USB_EHCI1   	14
#define IRQ_MFC        		15
#define IRQ_GMAC		16
#define IRQ_SDCI		17
#define IRQ_DMA         	18
#define IRQ_PL310           	19
#define IRQ_NAND		20
#define IRQ_SFI       		21
#define IRQ_USB_OHCI0   	22
#define IRQ_USB_OHCI1   	23
#define IRQ_USB0_OC         	24
#define IRQ_USB1_OC         	25
#define IRQ_UART0      		26  
#define IRQ_SPI0       		27
#define IRQ_IIC0        	28
#define IRQ_CSSYS		29
#define IRQ_BUS     	    	30
#define IRQ_EXT0		31


#endif

#endif /* #define _SDP1207_IRQS_H_ */
