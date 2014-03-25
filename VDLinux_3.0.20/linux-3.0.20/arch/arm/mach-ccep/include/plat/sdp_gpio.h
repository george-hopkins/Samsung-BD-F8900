/*
 * linux/arch/arm/mach-ccep/sdp_gpio.h
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : seunjgun.heo@samsung.com
 *
 */
#ifndef __SDP_GPIO_H
#define __SDP_GPIO_H

struct _sdp_gpio_pull
{
	unsigned char port;
	unsigned char pin;
	unsigned char regoffset;
	unsigned char regshift;
};

#if defined(CONFIG_ARCH_SDP92)		//Valencia
#define REG_GPIO_SIZE 0x200
#define REG_GPIO_CFG 0xD0
#define REG_GPIO_READ 0xD8
#define REG_GPIO_WRITE 0xD4
#elif defined(CONFIG_ARCH_SDP1001)	//GenoaS
#define REG_GPIO_SIZE 0x200
#define REG_GPIO_CFG 0xD0
#define REG_GPIO_READ 0xD8
#define REG_GPIO_WRITE 0xD4
#define REG_MGPIO_SIZE 0x100
#define REG_MGPIO_CFG 0x18
#define REG_MGPIO_READ 0x20
#define REG_MGPIO_WRITE 0x1C
#define MGPIO_START	16
#elif defined(CONFIG_ARCH_SDP1002)	//GenoaP
#define REG_GPIO_SIZE 0x100
#define REG_GPIO_CFG 0x44
#define REG_GPIO_READ 0x4C
#define REG_GPIO_WRITE 0x48
#define REG_MGPIO_SIZE 0x100
#define REG_MGPIO_CFG 0x18
#define REG_MGPIO_READ 0x20
#define REG_MGPIO_WRITE 0x1C
#define MGPIO_START	16
#elif defined(CONFIG_ARCH_SDP1103)	//Echo-s
#define REG_GPIO_SIZE 0x200
#define REG_GPIO_CFG 0x100
#define REG_GPIO_READ 0x108
#define REG_GPIO_WRITE 0x104
#elif defined(CONFIG_ARCH_SDP1105)	//Echo-B
#define REG_GPIO_SIZE 0x100 
#define REG_GPIO_CFG 0x30 
#define REG_GPIO_READ 0x38
#define REG_GPIO_WRITE 0x34 
#elif defined(CONFIG_ARCH_SDP1106)	//Echo-P
#define REG_GPIO_SIZE 0x200
#define REG_GPIO_CFG 0x100
#define REG_GPIO_READ 0x108
#define REG_GPIO_WRITE 0x104
#elif defined(CONFIG_ARCH_SDP1114)	//Echo-E
#define REG_GPIO_SIZE 0x200
#define REG_GPIO_CFG 0x100
#define REG_GPIO_READ 0x108
#define REG_GPIO_WRITE 0x104
#elif defined(CONFIG_ARCH_SDP1202)	//FoxAP
#define REG_GPIO_SIZE 0x400
#define REG_GPIO_CFG 0xF4
#define REG_GPIO_READ 0xFC
#define REG_GPIO_WRITE 0xF8
#elif defined(CONFIG_ARCH_SDP1207)	//FoxB
#define REG_GPIO_SIZE 0x200 
#define REG_GPIO_CFG 0x90
#define REG_GPIO_READ 0x98
#define REG_GPIO_WRITE 0x94 
#define REG_MGPIO_SIZE 0x100
#define REG_MGPIO_CFG 0x30
#define REG_MGPIO_READ 0x38
#define REG_MGPIO_WRITE 0x34
#define MGPIO_START	13
#else
#error  "Platform is not defined!!!"
#endif

#define GPIONUM2PORTPIN(num, port, pin)	do {port = ((num) & 0x1F0) >> 4; pin = (num) & 0xF;} while(0)


#define SDP_GPIO_MAJOR 120

#define GPIO_IOC_CONFIG		'J'
#define GPIO_IOC_WRITE		'K'
#define GPIO_IOC_READ		'L'

#define SDP_GPIO_FN		0
#define SDP_GPIO_IN		1
#define SDP_GPIO_OUT	2

#define SDP_GPIO_PULLUP		1
#define SDP_GPIO_PULLDOWN	0
#define SDP_GPIO_PULLUP_DISABLE	2

struct sdp_gpio_t {
	unsigned short	gpioNum;			// [3:0] pin, [8:4] port
	unsigned char	gpioFunc;			// 0:main function 2:in 3:out
	unsigned char	gpioPullUpDn;		// 0:Down 1:up
	unsigned char 	gpiolevel;			// 0 or 1, input or output
	unsigned char	reserve[3];			// reserved
};


#endif // __SDP_GPIO_H
