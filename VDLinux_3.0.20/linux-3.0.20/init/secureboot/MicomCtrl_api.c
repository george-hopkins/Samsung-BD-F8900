#include <linux/termios.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include "include/Secureboot.h"

#define _QUOTE(x)   #x
#define QUOTE(x)    _QUOTE(x)
#define MICOM_DEV_NAME  QUOTE(MICOM_DEV)

#define BAUDRATE B9600

//extern long sys_read(unsigned int fd, char *buf, long count);
//extern int printk(const char * fmt, ...);
//extern void * memcpy(void *, const void *, unsigned int);
//extern long sys_close(unsigned int fd);
//extern long sys_open(const char *filename, int flags, int mode);
//extern long sys_write(unsigned int fd, const char __attribute__((noderef, address_space(1))) *buf, long count);                              
//extern void msleep(unsigned int msecs);  

static void bzero(void * buf,int size)
{
	char * p=buf;
	int i;
	for(i=0;i<size;i++)
	{
		*p=0;
		p++;
	}
}

static int SerialInit(char *dev)
{
	int fd;
	
	fd = sys_open(dev, O_RDWR | O_NOCTTY,0 );//MS PARK
	if (fd <0) 
	{
		printk("Error.............. %s\n", dev);
		return (-1);
	}

	printk("open %s success\n", dev);
	return fd;
}

static int WriteSerialData(int fd, unsigned char *value, unsigned char num)
{
	int res;
	
	res = sys_write((unsigned int)fd, value, num);
	
	if (!res)
	{
		printk("Write Error!!\n");
	}
#ifdef POCKET_TEST
	if(!res)
	{
		printk("[!@#]MI_Err, ------------, -------\r\n");
    }
#endif // #ifdef POCKET_TEST
	return res;
}

static int SerialClose(int fd)
{
	int retVar = 0;

	if(sys_close((unsigned int)fd))
    {
        retVar = 1;
	}
	return retVar;
}

#define SERIAL_LEN	20
int MicomCtrl(unsigned char ctrl)
{
	int fd, len;
	char serial_target[SERIAL_LEN];
	unsigned char databuff[9];

	memset(databuff, 0, 9);
	bzero(serial_target, 20);

	len = strlen(MICOM_DEV_NAME);
	//if( len > SERIAL_LEN )
	//	len = SERIAL_LEN;

	strncpy(serial_target, MICOM_DEV_NAME, len);
	serial_target[len] = '\0';
	fd = SerialInit(serial_target);

	databuff[0] = 0xff;
	databuff[1] = 0xff;
	databuff[2] = ctrl;
	databuff[8] += databuff[2];

	WriteSerialData(fd, databuff, 9);
	msleep(10);
	SerialClose(fd);

	return 0;
}

#ifdef CONFIG_SUPPORT_REBOOT

#if CONFIG_NVT_NT72568
#define HAL_UART_REG_BASE           0x1D092000
#define HAL_UART_LSR_THREMPTY       0x00000020

#define HAL_READ_UINT32( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned int *)(_register_)))

#define HAL_WRITE_UINT8( _register_, _value_ ) \
        (*((volatile unsigned char *)(_register_)) = (_value_))
#endif

#ifdef CONFIG_MSTAR_CHIP // Mstar Chip
#include <asm/io.h>
#define REG_UART1_BASE              0x1F220C00 //UART 1

#define UART1_REG8(base, addr)            *((volatile unsigned int*)(base + ((addr)<< 3)))
#define UART_REG8_MICOM(base, addr)       UART1_REG8(base, addr) 
#define UART_TX                     0 
#define UART_LSR                5           // In:  Line Status Register
#define UART_LSR_THRE               0x20                                // Transmit-hold-register empty
#define UART_LSR_TEMT               0x40                                // Transmitter empty

#define BOTH_EMPTY                  (UART_LSR_TEMT | UART_LSR_THRE)
#endif  // end if CONFIG_MSTAR_CHIP // Mstar Chip

#ifdef CONFIG_ARCH_SDP1202
#include <asm/io.h>
#define PA_UART_BASE 		(0x10090A00)
#define UART_PORT_MICOM 	2
#endif 

void __iomem        *micom_base;
// You must implement micom reboot for each project!!!
int micom_reboot(void)
{
#ifdef CONFIG_MSTAR_CHIP // Mstar Chip
	unsigned char  u8Reg;
#endif
#ifdef CONFIG_NVT_NT72568
	unsigned long status=0;
#endif
	unsigned char databuff[9];
	int i;

	databuff[0] = 0xff;
	databuff[1] = 0xff;
	databuff[2] = 0x1d;
	databuff[3] = 0;

	databuff[4] = 0;
	databuff[5] = 0;
	databuff[6] = 0;
	databuff[7] = 0;
	databuff[8] = databuff[2];
	databuff[8] += databuff[3];
	
	printk("[SABSP] MICOM REBOOT EXECUTION!!!\n");

#ifdef CONFIG_MSTAR_CHIP // Mstar Chip(X12, X13)
	micom_base = ioremap(REG_UART1_BASE, 1024);
	printk("[SABSP] micom_base:0x%x(PHY : %x)\n", micom_base, REG_UART1_BASE);
	while(1)
	{
		for(i=0;i<9;i++)
		{
			printk("[SABSP] [%d] 1st \n", i);
			do {
				u8Reg = UART_REG8_MICOM(micom_base, UART_LSR);
				if ((u8Reg & UART_LSR_THRE) == UART_LSR_THRE)
				{
					break;
				}
			}while(1);

			printk("[SABSP] [%d] 2nd \n", i);
			(*(volatile unsigned int*)micom_base)= databuff[i];
			printk("[SABSP] [%d] 3nd \n", i);

			do {
				u8Reg = UART_REG8_MICOM(micom_base, UART_LSR);
				if ((u8Reg & BOTH_EMPTY) == BOTH_EMPTY)
				{
					break;
				}
			}while(1);
			printk("[SABSP] [%d] 4th \n", i);
		}
		printk("[SABSP] END~\n");
		msleep(100);
	}
#elif CONFIG_NVT_NT72568
	micom_base = ioremap(HAL_UART_REG_BASE, 1024);
	printk("[SABSP] micom_base:0x%x(PHY : %x)\n", micom_base,(HAL_UART_REG_BASE));
	while(1)
	{
		for(i=0;i<9;i++)
		{
			#if 0
			HAL_READ_UINT32(micom_base+0x14, status);
			printk("[SABSP] [%d] 2nd 0x%x \n", i, status);
			while (!(status & HAL_UART_LSR_THREMPTY))
				HAL_READ_UINT32(micom_base+0x14, status); 
			#endif
			printk("[SABSP] [%d] \n", i);
			HAL_WRITE_UINT8(micom_base, databuff[i]); 
		}
		printk("[SABSP] END~\n");
		msleep(100);
	}
#elif CONFIG_ARCH_SDP1202
	micom_base = 0xfe090a00; 
	printk("[SABSP] micom_base:0x%x(PHY : %x)\n", (unsigned int)micom_base, PA_UART_BASE);
	while(1)
	{
		for(i=0;i<9;i++)
		{
			while((((*(volatile unsigned int*)(micom_base+(0x40*UART_PORT_MICOM)+0x18))& (1<<9)) == 0) == 0) /* polling TX micom */
			{
			}
			(*(volatile char*)(micom_base+(0x40*UART_PORT_MICOM)+0x20))= databuff[i];

		}
		printk("[SABSP] END~\n");
		msleep(100);
	}
#else
	printk("[SABSP] MICOM reboot is not implemented for this project\n");
	printk("[SABSP] Please implement micom reboot if you need\n");
#endif  // end if CONFIG_MSTAR_CHIP (Mstar chip)

	return 0;
}

EXPORT_SYMBOL(micom_reboot);
#endif
