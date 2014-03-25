/*
 * irqs-sdp1114.h
 *
 * Copyright (c) 2012 Samsung Electronics
 * SeungJun Heo <seungjun.heo@samsung.com>
 * Dongseok Lee <drain.lee@samsung.com>
 *
 */

#ifndef _SDP1114_IRQS_H_
#define _SDP1114_IRQS_H_

#ifdef CONFIG_ARM_GIC

#define IRQ_PPI(x)	((x) + 16)
#define IRQ_LOCALTIMER	IRQ_PPI(13)

#define IRQ_SPI(x)	((x) + 32)

#define NR_IRQS			512//IRQ_SPI(96)	

#define IRQ_TSD			IRQ_SPI(0)
#define IRQ_AIO			IRQ_SPI(1)
#define IRQ_AE			IRQ_SPI(2)
#define IRQ_MPEG0		IRQ_SPI(3)
#define IRQ_MPEG1		IRQ_SPI(4)
#define IRQ_DISP		IRQ_SPI(5)
#define IRQ_GA2D		IRQ_SPI(6)
#define IRQ_GA3D0		IRQ_SPI(7)
#define IRQ_GA3D1		IRQ_SPI(8)
#define IRQ_GA3D2		IRQ_SPI(9)
#define IRQ_GA3D3		IRQ_SPI(10)
#define IRQ_GA3D4		IRQ_SPI(11)
#define IRQ_GA3D5		IRQ_SPI(12)
#define IRQ_GA3D6		IRQ_SPI(13)
#define IRQ_GA3D7		IRQ_SPI(14)
#define IRQ_GA3D8		IRQ_SPI(15)
#define IRQ_GA3D9		IRQ_SPI(16)
//#define IRQ_PCIE 		IRQ_SPI(17) /* not connected */
#define IRQ_MFD			IRQ_SPI(18)
#define IRQ_AVD			IRQ_SPI(19)
#define IRQ_FROAD 		IRQ_SPI(20)
#define IRQ_CSSYS		IRQ_SPI(21)
#define IRQ_CPUDMA		IRQ_SPI(22)
#define IRQ_EHCI0		IRQ_SPI(23)
#define IRQ_EMAC		IRQ_SPI(24)
#define IRQ_EHCI1		IRQ_SPI(25)
#define IRQ_SE			IRQ_SPI(26)
#define IRQ_TSMUX 		IRQ_SPI(27)
#define IRQ_UART		IRQ_SPI(28)
#define IRQ_UART0		IRQ_SPI(28)
#define IRQ_UART1		IRQ_SPI(29)
#define IRQ_UART2		IRQ_SPI(30)
#define IRQ_UART3		IRQ_SPI(31)
#define IRQ_OHCI0		IRQ_SPI(32)
#define IRQ_OHCI1		IRQ_SPI(33)
#define IRQ_HDMIRX		IRQ_SPI(34)
#define IRQ_TIMER		IRQ_SPI(35)
#define IRQ_TIMER0		IRQ_SPI(35)
#define IRQ_TIMER1		IRQ_SPI(36)
#define IRQ_TIMER2		IRQ_SPI(37)
#define IRQ_TIMER3		IRQ_SPI(38)
#define IRQ_I2C			IRQ_SPI(39)
#define IRQ_I2C0		IRQ_SPI(39)
#define IRQ_I2C1		IRQ_SPI(40)
#define IRQ_I2C2		IRQ_SPI(41)
#define IRQ_I2C3		IRQ_SPI(42)
#define IRQ_I2C4		IRQ_SPI(43)
#define IRQ_I2C5		IRQ_SPI(44)
#define IRQ_SCI			IRQ_SPI(45)
#define IRQ_DSP			IRQ_SPI(46)
#define IRQ_MMCIF		IRQ_SPI(47)
#define IRQ_OSDP		IRQ_SPI(48)
#define IRQ_IRR			IRQ_SPI(49)
#define IRQ_RTC			IRQ_SPI(50)
#define IRQ_IRB			IRQ_SPI(51)
#define IRQ_SMCDMA		IRQ_SPI(52)
#define IRQ_AMSDMA		IRQ_SPI(53)
#define IRQ_EXT0		IRQ_SPI(54)
#define IRQ_EXT1		IRQ_SPI(55)
#define IRQ_EXT2		IRQ_SPI(56)
#define IRQ_EXT3		IRQ_SPI(57)
#define IRQ_EXT4		IRQ_SPI(58)
#define IRQ_EXT5		IRQ_SPI(59)
#define IRQ_EXT6		IRQ_SPI(60)
#define IRQ_EXT7		IRQ_SPI(61)
#define IRQ_EXT8		IRQ_SPI(62)
#define IRQ_EXT9 		IRQ_SPI(63)
#define IRQ_SSP 		IRQ_SPI(64)/* IRQ_SPI */

#else/*!CONFIG_ARM_GIC*/

#define NR_IRQS			32

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_AE       		2
#define IRQ_MPEG0    		3
#define IRQ_MPEG1    		4
#define IRQ_DISP     		5
#define IRQ_GA2D     		6
#define IRQ_GA3D     		7
//#define IRQ_PCIE 	  	 	8/* not connected */
#define IRQ_MFD     		9
#define IRQ_AVD     		10
#define IRQ_FROAD	  	 	11
#define IRQ_CSSYS 	  	 	12
#define IRQ_CPUDMA      	13
#define IRQ_EHCI0			14
#define IRQ_EMAC			15
#define IRQ_EHCI1			16
#define IRQ_SE				17
#define IRQ_TSMUX   		18
#define IRQ_UART    		19
#define IRQ_OHCI0	    	20
#define IRQ_OHCI1			21
#define IRQ_HDMIRX	  	 	22
#define IRQ_TIMER    		23
#define IRQ_I2C     		24
#define IRQ_SCI     		25
#define IRQ_DSP				26
#define IRQ_MMCIF      		27
#define IRQ_OSDP      		28
#define IRQ_MERGER0			29 /* IRQ_IRR, IRQ_RTC, IRQ_IRB, IRQ_SPI, IRQ_SMCDMA, IRQ_AMSDMA */
#define IRQ_MERGER1     	30 /* IRQ_EXT 0 ~ 7 */
#define IRQ_SUB   			31 /* IRQ_USB0_VBUSVALID, IRQ_USB1_VBUSVALID */

#endif/*CONFIG_ARM_GIC*/
#endif

