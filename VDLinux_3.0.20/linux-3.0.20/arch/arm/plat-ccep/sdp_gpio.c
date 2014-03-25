/* linux/arch/arm/plat-sdp/gpio.c
 *
 * Copyright (c) 2010 
 *	Seungjun Heo <seungjun.heo@samsung.com>
 *
 * SDP chips GPIO support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/syscalls.h>	//    COMMON_070618_2 
#include <asm/uaccess.h>
#include <linux/miscdevice.h>

//#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <plat/sdp_gpio.h>


#define SDP_GPIO_MISC

//#define DPRINTK_GPIO	printk
#define DPRINTK_GPIO(...)
#ifdef REG_MGPIO_SIZE
#define SELECT_GPIO(x)	do { if(x >= (MGPIO_START << 4)) { x -= (MGPIO_START << 4); bMicom = 1;} else {bMicom = 0;} } while(0)
#else
#define SELECT_GPIO(x)  bMicom = 0
#endif


extern struct _sdp_gpio_pull sdp_gpio_pull_list[];
extern int sdp_gpio_pull_size;
extern struct _sdp_gpio_pull sdp_mgpio_pull_list[];
#ifdef REG_MGPIO_SIZE
extern int sdp_mgpio_pull_size;
#endif

static char gpio_name[] = "sdp_gpio";

static unsigned int sdp_gpio_base[2] = {0,0};
static unsigned int sdp_gpio_reg_read[2];
static unsigned int sdp_gpio_reg_write[2];
static unsigned int sdp_gpio_reg_cfg[2];

static spinlock_t lock;

struct _sdp_gpio_pull *gpio_pullmap[511] = {NULL, };
struct _sdp_gpio_pull *mgpio_pullmap[255] = {NULL, };
struct _sdp_gpio_pull **pullmap[2];

void sdp_gpio_cfgpull(unsigned int gpionum, unsigned int updown);
void sdp_gpio_cfgpin(unsigned int gpionum, unsigned int function);
void sdp_gpio_set_value(unsigned int gpionum, unsigned int value);
unsigned int sdp_gpio_get_value(unsigned int gpionum);

static long gpio_ioctl( struct file * file, unsigned int cmd, unsigned long arg);
static int gpio_open(struct inode *inode, struct file *filp);
static int gpio_release(struct inode *inode, struct file *filp);

static const struct file_operations sdp_gpio_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= gpio_ioctl,
	.open			= gpio_open,
	.release		= gpio_release,
};

static void sdp_gpio_getbase(void)
{
	int i;

	if(sdp_gpio_base[0] == 0)
	{
		sdp_gpio_base[0] = (unsigned int) ioremap(PA_PADCTRL_BASE, REG_GPIO_SIZE);
				
		//Create Pull Map
		for(i = 0 ; i < sdp_gpio_pull_size ; i++)
		{
			gpio_pullmap[(sdp_gpio_pull_list[i].port << 4) + sdp_gpio_pull_list[i].pin] = &(sdp_gpio_pull_list[i]);
		}		
		pullmap[0] = gpio_pullmap;
		sdp_gpio_reg_read[0] = REG_GPIO_READ;
		sdp_gpio_reg_write[0] = REG_GPIO_WRITE;
		sdp_gpio_reg_cfg[0] = REG_GPIO_CFG;

#ifdef REG_MGPIO_SIZE
		sdp_gpio_base[1]= (unsigned int) ioremap(PA_MPADCTRL_BASE, REG_MGPIO_SIZE);

		//Create Pull Map
		for(i = 0 ; i < sdp_mgpio_pull_size ; i++)
		{
			mgpio_pullmap[(sdp_mgpio_pull_list[i].port << 4) + sdp_mgpio_pull_list[i].pin] = &(sdp_mgpio_pull_list[i]);
		}		
		pullmap[1] = mgpio_pullmap;
		sdp_gpio_reg_read[1] = REG_MGPIO_READ;
		sdp_gpio_reg_write[1] = REG_MGPIO_WRITE;
		sdp_gpio_reg_cfg[1] = REG_MGPIO_CFG;
#endif
		spin_lock_init(&lock);
	}
}

void sdp_gpio_cfgpull(unsigned int gpionum, unsigned int updown)
{
	unsigned int addr;
	unsigned int value;
	unsigned long flags;
	int bMicom = 0;

	sdp_gpio_getbase();

	if(sdp_gpio_base[0] == 0)
	{
		DPRINTK_GPIO("Failed to Get GPIO Address\n");
		return;
	}

	if(updown > 2)
	{
		DPRINTK_GPIO("GPIO Invalid Argument updown = %d\n", updown);
		return;
	}

	SELECT_GPIO(gpionum);

	if(pullmap[bMicom][gpionum] == NULL)
	{
		unsigned int port, pin;
		GPIONUM2PORTPIN(gpionum, port, pin)	;
		printk("Not Exist GPIO (bMicom:%d port:%d  pin:%d)\n", bMicom, port, pin);
		BUG();
		return;
	}

	addr = sdp_gpio_base[bMicom] + pullmap[bMicom][gpionum]->regoffset;

	spin_lock_irqsave(&lock, flags);

//	value = readl(addr);

	value  = __raw_readl(addr);
	value &= (unsigned int)~(3 << (pullmap[bMicom][gpionum]->regshift));
	if(updown < SDP_GPIO_PULLUP_DISABLE)
	{
		value |= (updown + 2) << (pullmap[bMicom][gpionum]->regshift);
	}

	__raw_writel(value, addr);

	spin_unlock_irqrestore(&lock, flags);
	
	DPRINTK_GPIO("GPIO pull cfg : Num = %02X pull = %d, regoffset = %X, regshift = %d\n", gpionum, updown, pullmap[bMicom][gpionum]->regoffset, pullmap[bMicom][gpionum]->regshift);
}


void sdp_gpio_cfgpin(unsigned int gpionum, unsigned int function)
{
	unsigned int port;
	unsigned int pin;
	unsigned int addr;
	unsigned int value;
	unsigned long flags;
	int bMicom = 0;

	sdp_gpio_getbase();

	if(sdp_gpio_base[0] == 0)
	{
		DPRINTK_GPIO("Failed to Get GPIO Address\n");
		return;
	}

	if(function > 3)
	{
		DPRINTK_GPIO("GPIO Invalid Argument function = %d\n", function);
		return;
	}

	SELECT_GPIO(gpionum);
	GPIONUM2PORTPIN(gpionum, port, pin)	;

	addr = sdp_gpio_base[bMicom] + sdp_gpio_reg_cfg[bMicom] + (port * 12);

	spin_lock_irqsave(&lock, flags);

	//value = readl(addr);

	value  = __raw_readl(addr);
	value &=(unsigned int) ~(0x3 << (pin << 2));
	value |= (function ? (function+1) : 0) << (pin << 2);

	__raw_writel(value, addr);

	spin_unlock_irqrestore(&lock, flags);
	
	DPRINTK_GPIO("GPIO cfg : port = %d pin = %d fn = %d, Address = 0x%08X\n", port, pin, function, addr);
}

void sdp_gpio_set_value(unsigned int gpionum, unsigned int value)
{
	unsigned int port;
	unsigned int pin;
	unsigned int addr;
	unsigned long flags;
	unsigned int tmp;
	int bMicom = 0;
	
	sdp_gpio_getbase();

	if(sdp_gpio_base[0] == 0)
	{
		DPRINTK_GPIO("Failed to Get GPIO Address\n");
		return;
	}

	SELECT_GPIO(gpionum);
	GPIONUM2PORTPIN(gpionum, port, pin)	;

	addr = sdp_gpio_base[bMicom] + sdp_gpio_reg_write[bMicom] + (port * 12);

	spin_lock_irqsave(&lock, flags);

	tmp = __raw_readl(addr);
	tmp &= (unsigned int)~(1 << pin);
	tmp |= value << pin;
	__raw_writel(tmp, addr);

	spin_unlock_irqrestore(&lock, flags);

	DPRINTK_GPIO("GPIO Address = 0x%08X Value = %04XX\n", addr, tmp);

}

unsigned int sdp_gpio_get_value(unsigned int gpionum)
{
	unsigned int port;
	unsigned int pin;
	unsigned int addr;
	int bMicom = 0;
	
	sdp_gpio_getbase();

	if(sdp_gpio_base[0] == 0)
	{
		DPRINTK_GPIO("Failed to Get GPIO Address..\n");
		return -1;
	}

	SELECT_GPIO(gpionum);
	GPIONUM2PORTPIN(gpionum, port, pin)	;

	addr = sdp_gpio_base[bMicom] + sdp_gpio_reg_read[bMicom] + (port * 12);

	return (__raw_readl(addr) & (1 << pin)) >> pin;
}

static int gpio_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{	
	return 0;
}

static long gpio_ioctl(struct file * file, unsigned int cmd, unsigned long arg)
{
	struct sdp_gpio_t sIoData;
	
	if(copy_from_user(&sIoData, (void *) arg, sizeof(struct sdp_gpio_t)))
	{
		return -EFAULT;
	}

	switch(cmd)
	{
		case GPIO_IOC_CONFIG:
			sdp_gpio_cfgpin(sIoData.gpioNum, sIoData.gpioFunc);
			sdp_gpio_cfgpull(sIoData.gpioNum, sIoData.gpioPullUpDn);
			break;			
		case GPIO_IOC_WRITE:
			sdp_gpio_set_value(sIoData.gpioNum, sIoData.gpiolevel);
			break;
		case GPIO_IOC_READ:
			sIoData.gpiolevel =(unsigned char) sdp_gpio_get_value(sIoData.gpioNum);
			if(copy_to_user(&(((struct sdp_gpio_t *) arg)->gpiolevel), &(sIoData.gpiolevel), sizeof(sIoData.gpiolevel)))
			{
				return -EFAULT;
			}
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

#ifdef SDP_GPIO_MISC
#define GPIO_MINOR 120
static struct miscdevice sdp_gpio_dev = {
	GPIO_MINOR,
	gpio_name,
	&sdp_gpio_fops
};
#else
static struct cdev sdp_gpio_cdev = {
    .owner = THIS_MODULE,
    .ops = &sdp_gpio_fops,
};
#endif

#ifdef CONFIG_ARCH_SDP1202
#ifndef CONFIG_VD_RELEASE 
volatile unsigned int *g_sdprcu;
#endif
#endif

static int __init gpio_init(void)
{
	int res;
#ifdef CONFIG_ARCH_SDP1202
#ifndef CONFIG_VD_RELEASE 
	void *buf;
#endif
#endif
#ifdef SDP_GPIO_MISC
	res = misc_register(&sdp_gpio_dev);
	if (res)
	{
		printk(KERN_ERR "gpio: can't misc_register on minor=%d\n",
		    GPIO_MINOR);
		return res;
	}
#else		// 2.6 Char Device Driver
	dev_t dev;

	dev = MKDEV(SDP_GPIO_MAJOR, 0);
	res = register_chrdev_region(dev, 1, gpio_name);
	if(res < 0)
	{
		printk("[SDP_GPIO] register_chrdev_region Error\n");
		res = -ENODEV;
		return res;
	}

	cdev_init(&sdp_gpio_cdev, &sdp_gpio_fops);
	sdp_gpio_cdev.owner = THIS_MODULE;
	sdp_gpio_cdev.ops = &sdp_gpio_fops;
	
	if(cdev_add(&sdp_gpio_cdev, dev, 1))
	{
		printk("[SDP_GPIO] cdev_add Error\n");
		res = -ENODEV;
		return res;
	}
#endif

	sdp_gpio_getbase();

	printk("SDP GPIO Driver Init Finished...\n");
#ifdef CONFIG_ARCH_SDP1202
#ifndef CONFIG_VD_RELEASE 
	buf = ioremap(0x18400000, 0x100);
	g_sdprcu = buf + 0xac;
#endif
#endif
	return res;		
}
#ifdef CONFIG_ARCH_SDP1202
#ifndef CONFIG_VD_RELEASE 
EXPORT_SUMBOL(g_sdprcu);
#endif
#endif
EXPORT_SYMBOL(sdp_gpio_cfgpull);
EXPORT_SYMBOL(sdp_gpio_cfgpin);
EXPORT_SYMBOL(sdp_gpio_set_value);
EXPORT_SYMBOL(sdp_gpio_get_value);
module_init(gpio_init);


