/*
 * irqs-sdp1104fpga.h
 *
 * Copyright (c) 2011 Samsung Electronics
 * Seungjun Heo <seungjun.heo@samsung.com>
 *
 */

#ifndef _SDP1104_FPGA_IRQS_H_
#define _SDP1104_FPGA_IRQS_H_

#ifdef CONFIG_ARM_GIC

#define IRQ_PPI(x)	((x) + 16)
#define IRQ_LOCALTIMER	IRQ_PPI(13)

#define IRQ_SPI(x)	((x) + 32)

#define NR_IRQS         IRQ_SPI(128)	

#define IRQ_TSD         IRQ_SPI(0)                                 
#define IRQ_AIO         IRQ_SPI(1)                                 
#define IRQ_BDGA        IRQ_SPI(2)                                 
#define IRQ_TIMER       IRQ_SPI(3)                                 
#define IRQ_TIMER0      IRQ_SPI(3)                                 
#define IRQ_TIMER1      IRQ_SPI(4)                                 
#define IRQ_TIMER2      IRQ_SPI(5)                                 
#define IRQ_TIMER3      IRQ_SPI(6)                                 
#define IRQ_BDGA_RAW0   IRQ_SPI(7)                                 
#define IRQ_BDGA_RAW1   IRQ_SPI(8)                                 
#define IRQ_BDGA_RAW2   IRQ_SPI(9)                                 
#define IRQ_BDGA_RAW3   IRQ_SPI(10)                                
#define IRQ_BDGA_RAW4   IRQ_SPI(11)                                
#define IRQ_BDGA_RAW5   IRQ_SPI(12)                                
#define IRQ_BDGA_RAW6   IRQ_SPI(13)                                
#define IRQ_BDGA_RAW7   IRQ_SPI(14)                                
#define IRQ_BDGA_RAW8   IRQ_SPI(15)                                
#define IRQ_BDGA_RAW9   IRQ_SPI(16)                                
#define IRQ_BDGA_RAW10  IRQ_SPI(17)                                
#define IRQ_PSD         IRQ_SPI(18)                                
#define IRQ_SE          IRQ_SPI(19)                                
#define IRQ_AE          IRQ_SPI(20)                                
#define IRQ_HDMI        IRQ_SPI(21)                                
#define IRQ_SATA0       IRQ_SPI(22)                                
#define IRQ_SATA1       IRQ_SPI(23)                                
#define IRQ_RMI         IRQ_SPI(24)                                
#define IRQ_VDSP        IRQ_SPI(25)                                
#define IRQ_DISP        IRQ_SPI(26)                                
#define IRQ_EHCI0       IRQ_SPI(27)                                
#define IRQ_EHCI1       IRQ_SPI(28)                                
#define IRQ_MFD_tH      IRQ_SPI(29)                                
#define IRQ_GMAC        IRQ_SPI(30)                                
#define IRQ_SDMMC       IRQ_SPI(31)                                
#define IRQ_GDMA0_CPU   IRQ_SPI(32)                                
#define IRQ_GDMA1_CPU   IRQ_SPI(33)                                
#define IRQ_TSM         IRQ_SPI(34)                                
#define IRQ_NAND        IRQ_SPI(35)                                
#define IRQ_BOOT        IRQ_SPI(36)                                
#define IRQ_SMC         IRQ_SPI(37)                                
#define IRQ_OHCI0       IRQ_SPI(38)                                
#define IRQ_OHCI1       IRQ_SPI(39)                                
#define IRQ_USB0_OC     IRQ_SPI(40)                                
#define IRQ_USB1_OC     IRQ_SPI(41)                                
#define IRQ_UART        IRQ_SPI(42)                                
#define IRQ_UART0       IRQ_SPI(42)                                
#define IRQ_UART1       IRQ_SPI(43)                                
#define IRQ_UART2       IRQ_SPI(44)                                
#define IRQ_UART3       IRQ_SPI(45)                                
#define IRQ_UDMA0       IRQ_SPI(46)                                
#define IRQ_UDMA1       IRQ_SPI(47)                                
#define IRQ_SPI0        IRQ_SPI(48)                                
#define IRQ_I2C0        IRQ_SPI(49)                                
#define IRQ_I2C1        IRQ_SPI(50)                                
#define IRQ_I2C2        IRQ_SPI(51)                                
#define IRQ_I2C3        IRQ_SPI(52)                                
#define IRQ_I2C4        IRQ_SPI(53)                                
#define IRQ_XROAD       IRQ_SPI(54)                                
#define IRQ_USB3        IRQ_SPI(55)                                
#define IRQ_EXT0        IRQ_SPI(56)                                
#define IRQ_EXT1        IRQ_SPI(57)                                
#define IRQ_EXT2        IRQ_SPI(58)                                
#define IRQ_EXT3        IRQ_SPI(59)                                
#define IRQ_EXT4        IRQ_SPI(60)                                
#define IRQ_EXT5        IRQ_SPI(61)                                
#define IRQ_EXT6        IRQ_SPI(62)                                
#define IRQ_EXT7        IRQ_SPI(63)                                
#define IRQ_CSSYS0      IRQ_SPI(125)                               
#define IRQ_CSSYS1      IRQ_SPI(126)                               
#define IRQ_DMA330      IRQ_SPI(127)                               

#else

#if !defined(CONFIG_SDP_INTC64) 
#define NR_IRQS			32	

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_BDGA       		2
#define IRQ_TIMER    		3
#define IRQ_PSD    		4
#define IRQ_SE     		5
#define IRQ_AE     		6
#define IRQ_HDMI     		7

#define IRQ_SATA0 	   	8
#define IRQ_SATA1_PCIE 		9
#define IRQ_RMI     		10
#define IRQ_RESERVED	   	11
#define IRQ_BDDP 	   	12
#define IRQ_USB_EHCI0  		13
#define IRQ_USB_EHCI1		14
#define IRQ_MFD			15

#define IRQ_EMAC		16
#define IRQ_SDMMC		17
#define IRQ_GDMA		18
#define IRQ_PL310    		19
#define IRQ_NFCON    		20
#define IRQ_SMC			21
#define IRQ_USB_OHCI0   	22
#define IRQ_USB_OHCI1   	23

#define IRQ_USB0_OVERCUR 	24
#define IRQ_USB1_OVERCUR 	25
#define IRQ_UART     		26
#define IRQ_SPI     		27
#define IRQ_I2C			28
#define IRQ_CSSYS      		29
#define IRQ_BUS      		30
#define IRQ_EXT   		31	/* 0 ~ 7 */

#else	/* irq resource 64*/
#define NR_IRQS			64

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_BDGA       		2
#define IRQ_TIMER    		3
#define IRQ_PSD    		4
#define IRQ_SE     		5
#define IRQ_AE     		6
#define IRQ_HDMI     		7

#define IRQ_SATA0 	   	8
#define IRQ_SATA1_PCIE 		9
#define IRQ_GP     		10
#define IRQ_COP0	   	11
#define IRQ_BDDP 	   	12
#define IRQ_USB_EHCI0  		13
#define IRQ_USB_EHCI1		14
#define IRQ_MFD			15

#define IRQ_EMAC		16
#define IRQ_SDMMC		17
#define IRQ_GDMA		18
#define IRQ_HDMI_DDC    	19
#define IRQ_NFCON    		20
#define IRQ_COP1		21
#define IRQ_USB_OHCI0   	22
#define IRQ_USB_OHCI1   	23

#define IRQ_GA2D 		24
#define IRQ_GA3D 		25
#define IRQ_GSCL     		26
#define IRQ_RLD     		27
#define IRQ_PNG			28
#define IRQ_JPEG      		29
#define IRQ_CSC      		30
#define IRQ_SDP   		31	

#define IRQ_UART0		32
#define IRQ_UART1		33
#define IRQ_UART2		34
#define IRQ_UART3		35
#define IRQ_UDMA0		36
#define IRQ_UDMA1		37
#define IRQ_I2C1_4		38
#define IRQ_SPI			39

#define IRQ_SATA0_REDEF		40
#define IRQ_SATA1_PCIE_REDEF	41
#define IRQ_RMI			42
#define IRQ_RESERVED		43
#define IRQ_BUS			44
#define IRQ_CSSYS		45
#define IRQ_USB0_OVERCUR	46
#define IRQ_USB1_OVERCUR	47

#define IRQ_EMAC_REDEF		48
#define IRQ_SDMMC_REDEF		49
#define IRQ_GDMA_REDEF		50
#define IRQ_PL310		51
#define IRQ_NFCON_REDEF		52
#define IRQ_SMC			53
#define IRQ_USB_OHCI0_REDEF   	54
#define IRQ_USB_OHCI1_REDEF   	55

#define IRQ_EXTINT0		56
#define IRQ_EXTINT1		57
#define IRQ_EXTINT2		58
#define IRQ_EXTINT3		59
#define IRQ_EXTINT4		60
#define IRQ_EXTINT5		61
#define IRQ_EXTINT6		62
#define IRQ_EXTINT7		63

#endif /* CONFIG_SDP_INTC64 */

#endif /* CONFIG_ARM_GIC */

#endif /* _SDP1104_FPGA_IRQS_H_ */

