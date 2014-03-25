/*********************************************************************************************
 *
 *	sdp_unzipc.c (Samsung DTV Soc unzip device driver)
 *
 *	author : seungjun.heo@samsung.com
 *	
 ********************************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm/irq.h>

#include <linux/delay.h>
#include <plat/sdp_unzip.h>

#define R_GZIP_IRQ				0x00
#define R_GZIP_IRQ_MASK			0x04
#define R_GZIP_CMD				0x08
#define R_GZIP_IN_BUF_ADDR		0x00C
#define R_GZIP_IN_BUF_SIZE		0x010
#define R_GZIP_IN_BUF_POINTER	0x014
#define R_GZIP_OUT_BUF_ADDR		0x018
#define R_GZIP_OUT_BUF_SIZE		0x01C
#define R_GZIP_OUT_BUF_POINTER	0x020
#define R_GZIP_LZ_ADDR		0x024
#define R_GZIP_DEC_CTRL		0x28
#define R_GZIP_PROC_DELAY	0x2C
#define R_GZIP_TIMEOUT		0x30
#define R_GZIP_IRQ_DELAY	0x34
#define R_GZIP_FILE_INFO	0x38
#define R_GZIP_ERR_CODE		0x3C
#define R_GZIP_PROC_STATE	0x40
#define R_GZIP_ENC_DATA_END_DELAY	0x44
#define R_GZIP_CRC32_VALUE_HDL		0x48
#define R_GZIP_CRC32_VALUE_SW		0x4C
#define R_GZIP_ISIZE_VALUE_HDL		0x50
#define R_GZIP_ISIZE_VALUE_SW		0x54
#define R_GZIP_ADDR_LIST1			0x58

#define V_GZIP_CTL_ADVMODE	(0x1 << 24)
#define V_GZIP_CTL_ISIZE	(0x0 << 21)
#define V_GZIP_CTL_CRC32	(0x0 << 20)
#define V_GZIP_CTL_OUT_PAR	(0x1 << 12)
#define V_GZIP_CTL_IN_PAR	(0x1 << 8)
#define V_GZIP_CTL_OUT_ENDIAN_LITTLE	(0x1 << 4)
#define V_GZIP_CTL_IN_ENDIAN_LITTLE		(0x1 << 0)

#define GZIP_WINDOWSIZE	4		//(0:256, 1:512, 2:1024, 3:2048, 4:4096...
#define GZIP_ALIGNSIZE	64

static struct sdp_unzip_t
{
	int initialized;
	void *pLzBuf;
	struct semaphore mutex;
	wait_queue_head_t syncQueue;
	int syncCondition;
} sdp_unzip;
atomic_t sdp_busy;
EXPORT_SYMBOL(sdp_busy);

char * poobuf;
char * poobuf2;

extern void *kmap_atomic_pfn(unsigned long pfn);

extern int sdp_set_clockgating(u32 phy_addr, u32 mask, u32 value);

#ifdef CONFIG_SDP_UNZIP_TEST_DRIVER

#define UNZIP_MINOR	121

static char unzip_name[] = "sdp_unzip";

extern void show_irq(void);

static int unzip_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int unzip_release(struct inode *inode, struct file *filp)
{	
	return 0;
}

static long unzip_ioctl(struct file * file, unsigned int cmd, unsigned long arg)
{
	int i;
	char *ibuff;
	char *opages[32];
	struct sdp_unzip_arg_t sIoData;
	
	if(copy_from_user(&sIoData, (void *) arg, sizeof(struct sdp_unzip_arg_t)))
	{
		return -EFAULT;
	}

	switch(cmd)
	{
		case GZIP_IOC_DECOMPRESS:
			if(sIoData.size_ibuff > (int) (128 << 10))	//input buffer Max size is 128KB
				return (long) -EINVAL;
			ibuff = kmalloc((size_t) ((sIoData.size_ibuff + GZIP_ALIGNSIZE - 1) / GZIP_ALIGNSIZE) * GZIP_ALIGNSIZE, GFP_ATOMIC);
			if(ibuff == NULL)
				return (long) -EFAULT;
			if(copy_from_user(ibuff, sIoData.ibuff, (unsigned long) sIoData.size_ibuff))
			{
				kfree(ibuff);
				return (long) -EFAULT;
			}
			opages[0] = kmalloc(4096 * 32, GFP_ATOMIC);
			if(opages[0] == NULL)
			{
				kfree(ibuff);			
				return (long) -EFAULT;
			}
			for(i = 0; i < sIoData.npages; i++)
			{
				opages[i] = opages[0] + 4096 * i;
			}
			if(sdp_unzip_decompress(ibuff, sIoData.size_ibuff, opages, sIoData.npages) < 0)
			{
				pr_err("unzip Error!!!\n");
			}
			for(i = 0; i < sIoData.npages; i++)
			{
				if(copy_to_user(sIoData.opages[i], opages[i], 4096))
				{
					kfree(opages[0]);
					kfree(ibuff);			
					return (long) -EFAULT;
				}
			}
			kfree(opages[0]);
			kfree(ibuff);			
			break;			
#if 0
		case GZIP_IOC_DECOMPRESS_AGING:
			{
				struct sdp_unzip_aging_arg_t sIoData2;
				char *ibuff[20];
				int ncnt = 0;
				struct timeval start, end;
				u32 diff, size;
				
				if(copy_from_user(&sIoData2, (void *) arg, sizeof(struct sdp_unzip_aging_arg_t)))
				{
					return -EFAULT;
				}
				for (i = 0; i < sIoData2.nbufs ; i++)
				{
					ibuff[i] = kmalloc(sIoData2.ibuffsize[i], GFP_ATOMIC);
					if(copy_from_user(ibuff[i], sIoData2.ibuffes[i], sIoData2.ibuffsize[i]))
					{
						return -EFAULT;
					}
				}
				opages[0] = kmalloc(4096 * 32, GFP_ATOMIC);
				for(i = 1; i < 32; i++)
				{
					opages[i] = opages[0] + 4096 * i;
				}
				while(1)
				{
					if((ncnt % 100) == 0)
					{
						do_gettimeofday(&start);
					}
					for(i = 0; i < sIoData2.nbufs ; i++)
					{
						if(sdp_unzip_decompress(ibuff[i], sIoData2.ibuffsize[i], opages, 32) < 0)
						{
							pr_err("unzip Error!!!\n");
						}
					}
					ncnt++;
					if((ncnt % 100) == 0)
					{
						do_gettimeofday(&end);
						diff = (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec);
						size = 128 * sIoData2.nbufs * 100;
						printk("%d th running......\n", ncnt);
						printk("throughput : %dMB/s\n", size * 1000 / diff);
					}
				}
				kfree(opages[0]);
				for(i = 0 ; i < sIoData2.nbufs ;i++)
					kfree(ibuff[i]);			
			}
			break;			
#endif
		default:
			return (long) -EINVAL;
	}
	return 0;
}

static const struct file_operations sdp_unzip_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= unzip_ioctl,
	.open			= unzip_open,
	.release		= unzip_release,
};

static struct miscdevice sdp_unzip_dev = {
	.minor	= UNZIP_MINOR,
	.name	= unzip_name,
	.fops	= &sdp_unzip_fops
};

#endif

static irqreturn_t unzip_interrupt_handler(int irq, void* devId)
{
	u32 value;

	value = readl(VA_UNZIP_BASE + R_GZIP_IRQ);
	writel(value, VA_UNZIP_BASE + R_GZIP_IRQ);

	if((value == 0) || (value & 0x8))
	{
		pr_err(KERN_ERR "unzip: unzip interrupt flags=%d errorcode=0x%08X\n"
			, value, readl(VA_UNZIP_BASE + R_GZIP_ERR_CODE));
	}
	sdp_unzip.syncCondition = (int) value;
	wake_up_interruptible(&sdp_unzip.syncQueue);

	return IRQ_HANDLED;
	
}

int sdp_unzip_init(void)
{
	void *lzbuf;

#ifdef CONFIG_SDP_UNZIP_TEST_DRIVER
	int res;

	res = misc_register(&sdp_unzip_dev);
	if (res)
	{
		pr_err(KERN_ERR "unzip: can't misc_register on minor=%d\n",
		    UNZIP_MINOR);
		return res;
	}

	printk("SDP Unzip Test Driver Init Finished...\n");
#endif

	if(sdp_unzip.initialized)
		return 0;			//already initialized

	sdp_busy.counter = 0;

	lzbuf = kmalloc(4096, GFP_KERNEL);

	if(lzbuf == NULL)
	{
		pr_err("cannot allocate buffer for LZ!!!\n");
		return 0;
	}
	
	sdp_unzip.pLzBuf = (void *) (u32) __pa(lzbuf);
	if(request_irq(IRQ_GZIP, unzip_interrupt_handler, 0, "sdp_unzip_irq", (void *) NULL))
	{
		pr_err("request_irq Fail!!!!\n");
		kfree(lzbuf);
		return 0;
	}
#ifdef CONFIG_ARCH_SDP1202
	if(num_online_cpus() > 1)
		irq_set_affinity(IRQ_GZIP, cpumask_of(1));
#endif

	sema_init(&sdp_unzip.mutex, 1);
	init_waitqueue_head(&sdp_unzip.syncQueue);

	sdp_unzip.initialized = 1;

	printk("SDP Unzip Engine Init Finished...\n");

#ifdef CONFIG_ARCH_SDP1202
	sdp_set_clockgating(0x10090954, 0x00800000, 0);
	sdp_set_clockgating(0x10090944, 0x20000000, 0);
#endif
	
	return 0;
}

int sdp_unzip_decompress(char *ibuff, int ilength, char *opages[], int npages)
{
	int i, ret, err = npages * 4096;
	u32 value;
	void *pibuff, *pobuff;

	if(!sdp_unzip.initialized)
	{
		printk("SDP Unzip Engine is not Initialized!!!\n");
		return -1;
	}

	atomic_inc(&sdp_busy);
	down(&sdp_unzip.mutex);

#ifdef CONFIG_ARCH_SDP1202
	sdp_set_clockgating(0x10090944, 0x20000000, 0x20000000);
	sdp_set_clockgating(0x10090954, 0x00800000, 0x00800000);
#endif
	
	writel(0, VA_UNZIP_BASE + R_GZIP_CMD);			//Gzip Reset

	sdp_unzip.syncCondition = 0;

	pibuff = (void *) (u32) __pa(ibuff);

	//Set Source
	writel(pibuff, VA_UNZIP_BASE + R_GZIP_IN_BUF_ADDR);		//Set Src Address
	ilength = ((ilength + GZIP_ALIGNSIZE ) / GZIP_ALIGNSIZE) * GZIP_ALIGNSIZE;
	writel(ilength, VA_UNZIP_BASE + R_GZIP_IN_BUF_SIZE);	//Set Src Size
	writel(sdp_unzip.pLzBuf, VA_UNZIP_BASE + R_GZIP_LZ_ADDR);	//Set LZ Buf Address

	dmac_map_area(ibuff, (size_t) ilength, DMA_TO_DEVICE);
	outer_clean_range((u32) pibuff, (u32) pibuff + (u32) ilength);			//Cache Clean

	//Set Destination
	writel(__pa(opages[0]), VA_UNZIP_BASE + R_GZIP_OUT_BUF_ADDR);	//Set Dst Address 0
	for(i = 1 ; i < npages; i++)
	{
		writel(__pa(opages[i]), VA_UNZIP_BASE + R_GZIP_ADDR_LIST1 + (u32) ((i-1)*4));	//Set Dst Address
	}
	writel(4096, VA_UNZIP_BASE + R_GZIP_OUT_BUF_SIZE);	//Set Dst Size

	value = GZIP_WINDOWSIZE << 16;
	value |= V_GZIP_CTL_OUT_PAR | V_GZIP_CTL_ADVMODE;
	value |= 0x11;
	
	for(i = 0 ; i < npages ; i++)
	{
		pobuff = (void *) (u32) __pa(opages[i]);
		dmac_map_area(opages[i], 4096, DMA_FROM_DEVICE);
		outer_inv_range((u32) pobuff, (u32) pobuff + 4096);
	}

	writel(value, VA_UNZIP_BASE + R_GZIP_DEC_CTRL);	//Set Decoding Control Register
	writel(0xffffffff, VA_UNZIP_BASE + R_GZIP_TIMEOUT);	//Set Timeout Value
	writel(0xffffffff, VA_UNZIP_BASE + R_GZIP_IRQ_MASK);	//Set IRQ Mask Register
	writel(1, VA_UNZIP_BASE + R_GZIP_CMD);		//Start Decoding

	ret = wait_event_interruptible_timeout(sdp_unzip.syncQueue, sdp_unzip.syncCondition, HZ);

	if(ret == -ERESTARTSYS)
	{
		u64 timeout;
		
//		pr_err("unzip : -ERESTARTSYS is caused!!\n");
		timeout = jiffies + HZ;
		while(!sdp_unzip.syncCondition)
		{
			cond_resched();
			if(jiffies > timeout)
			{
				pr_err("unzip : timeout error!!!!!\n");
				err = -1;
				break;
			}
		}
		
	}
	else if(ret <= 0)
	{
		pr_err("unzip interrupt error!!!! ret = %d\n", ret);
		err = -1;
	}

	if(sdp_unzip.syncCondition & 0x8)			//Check Error Flag
	{
		pr_err("unzip Error Occur!!!!! %d code=0x%08X\n", sdp_unzip.syncCondition, readl(VA_UNZIP_BASE + R_GZIP_ERR_CODE));
		err = -1;
	}

	if(!(sdp_unzip.syncCondition & 0x1))		//Check Decoding Done
	{
		pr_err("Cannot Finish Decoding!!!!! %d code=0x%08X\n", sdp_unzip.syncCondition, readl(VA_UNZIP_BASE + R_GZIP_ERR_CODE));
		err = -1;
	}

	for(i = 0 ; i < npages; i++)
	{
		pobuff = (void *) __pa(opages[i]);
		outer_inv_range((u32) pobuff, (u32) pobuff + 4096);
		dmac_unmap_area(opages[i], 4096, DMA_FROM_DEVICE);		//Cache Invalidate
	}

	if(err < 0)		//if error dump input buffer and gzip register
	{
		int j, cnt;
		volatile unsigned int *ibuf;
		char *pbuf = NULL, *vbuf = NULL;
		pbuf = kmalloc((size_t) ilength, GFP_ATOMIC);
		vbuf = kmalloc((size_t) ilength, GFP_ATOMIC);
		if((pbuf != NULL) && (vbuf != NULL))
		{
			printk("-------------DUMP GZIP registers------------\n");
			for(i = 0; i < 0xDF ; i += 0x10)
			{
				for(j = 0 ; j < 0x10 ; j += 4)
				{
					printk("0x%08X ", readl(VA_UNZIP_BASE + (u32) (i + j)));
				}
				printk("\n");
			}
			printk("--------------------------------------------\n");
			printk("-------------DUMP Input Buffer--------------\n");
			printk("------- Virtual address : 0x%08X -------\n", (unsigned int) ibuff);
			ibuf = (volatile unsigned int *)(((unsigned int) ibuff) & 0xFFFFFFE0);
			cnt = 0;
			printk("0x%08X : \n", (unsigned int) ibuf);
			while(1)
			{
				if(((u32) ibuf) < ((u32)ibuff))
				{
					printk("           ");
					ibuf++;
					cnt++;
					continue;
				}
				printk("0x%08X ", *ibuf);
				if((cnt % 8) == 7)
					printk("\n");
				ibuf++;
				cnt++;
				if(((u32) ibuf) >= ((u32)(ibuff + 0x400)))
				{
					printk("\n");
					break;
				}
			}
			memcpy(vbuf, ibuff, (size_t) ilength);
			
			printk("------- Physical address : 0x%08X -------\n", (unsigned int) pibuff);
			dmac_unmap_area(ibuff, (size_t) ilength, DMA_FROM_DEVICE);
			outer_inv_range((u32) pibuff, (u32)pibuff + (u32) ilength);			//Cache Invalidate
			ibuf = (volatile unsigned int *)(((unsigned int) ibuff) & 0xFFFFFFE0);
			cnt = 0;
			memcpy(pbuf, ibuff, (size_t) ilength);
			if(memcmp(pbuf, vbuf, (size_t) ilength))
			{
				printk("Virtual Buffer is different from Physical!!!!!\n");
			}
			else
			{
				printk("Virtual Buffer is same to Physical\n");
			}
			printk("--------------------------------------------\n");
			kfree(pbuf);
			kfree(vbuf);
			show_irq();
		}
		else if(pbuf != NULL)
		{
			kfree(pbuf);
		}
		else if(vbuf != NULL)
		{
			kfree(vbuf);
		}
	}
	writel(0, VA_UNZIP_BASE + R_GZIP_CMD);			//Gzip Reset
#ifdef CONFIG_ARCH_SDP1202
	sdp_set_clockgating(0x10090954, 0x00800000, 0);
	sdp_set_clockgating(0x10090944, 0x20000000, 0);
#endif

	up(&sdp_unzip.mutex);
	atomic_dec(&sdp_busy);
	//printk(" sdp_unzip_decompress err = %d\n",err);

	return err;
}
EXPORT_SYMBOL(sdp_unzip_init);
#ifdef CONFIG_SDP_UNZIP_TEST_DRIVER

module_init(sdp_unzip_init);

#endif
