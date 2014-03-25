#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

#include <asm/io.h>
#include <plat/sdp_smc.h>

#undef SDP_SMC_NEED_LOCK
#define SDP_SMC_MAX_BANKS	4

#define SDPSMC_STATE_UNUSED	0
#define SDPSMC_STATE_INITED	1

struct smc_dma_reg
{
	u32 flash;
	u32 timing;
	u32 target;
	u32 control;
	u32 intcon;
} __attribute__((packed));

struct sdp_smc
{
	spinlock_t			lock;
	struct kref			kref;
	struct smc_dma_reg __iomem	*dma_regs;
	struct device			*dev;
	resource_size_t			remapped_addr;
	resource_size_t			remapped_len;
	struct sdp_smc_bank		bank[SDP_SMC_MAX_BANKS];	
};

/* copy by 4 * 8 words, onenand 16 burst */
static void smc_memcpy128(void* dst, void *src, size_t len) noinline;
static void smc_memcpy128(void* dst, void *src, size_t len)
{ 
	asm volatile (
		"1:\n\t"
		"ldmia	%1!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"stmia	%0!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"ldmia	%1!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"stmia	%0!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"ldmia	%1!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"stmia	%0!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"ldmia	%1!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"stmia	%0!, {r0,r1,r2,r3,r4,r5,r6,r7}"	"\n\t"
		"subs	%2, %2, #128"	"\n\t"
		"bgt	1b\n\t"
		: "=r"(dst), "=r"(src), "=r"(len)
		: "0"(dst), "1"(src), "2"(len)
		: "r0","r1","r2","r3","r4","r5","r6","r7","cc"
	);
}

static void smc_memcpy2(void* dst, void* src, size_t len)
{ 
	unsigned short tmp = 0;

	asm volatile (
		"1:\n\t"
		"ldrh	%3, [%1], #2"	"\n\t"
		"subs	%2, %2, #2"	"\n\t"
		"strh	%3, [%0], #2"	"\n\t"
		"bgt	1b"		"\n\t"
		: "=r"(dst), "=r"(src), "=r"(len), "=r"(tmp)
		: "0"(dst), "1"(src), "2"(len), "3"(tmp)
		: "cc"
	);
}

static void smc_memcpy1(void* dst, void* src, size_t len)
{ 
	unsigned char tmp = 0;

	asm volatile (
		"1:\n\t"
		"ldrb	%3, [%1], #1"	"\n\t"
		"subs	%2, %2, #1"	"\n\t"
		"strb	%3, [%0], #1"	"\n\t"
		"bgt	1b"		"\n\t"
		: "=r"(dst), "=r"(src), "=r"(len), "=r"(tmp)
		: "0"(dst), "1"(src), "2"(len)
		: "cc"
	);
}

static void smcdma_xfer (struct sdp_smc *smc, u32 dst, u32 src, size_t len)
{
	u32 v;
	
	/* no argument check */	
	v = readl(&smc->dma_regs->flash);
	v &= 0xfff00000;
	v |= (len<<7) | src;
	writel (v, &smc->dma_regs->flash);

	writel (dst, &smc->dma_regs->target);

	/* start DMA */
	v = readl (&smc->dma_regs->control);
	writel (v | 0x8, &smc->dma_regs->control);	/* stop first for safety */
	writel (v | 0x1, &smc->dma_regs->control);	/* enable */
	writel (v | 0x3, &smc->dma_regs->control);	/* start */

	/* wait for completed */
	do {
		v = readl (&smc->dma_regs->intcon);
	} while (!(v&1));

	v = readl (&smc->dma_regs->control);
	writel (v | 0x8, &smc->dma_regs->control);	/* stop */
}

static void *smc_memcpy(void *dst, void *src, size_t len)
{
	unsigned int dstl, srcl;
	dstl = (unsigned int)dst;
	srcl = (unsigned int)src;

	if ( !(dstl & 3) && !(srcl & 3) && !(len & 127)) {
		smc_memcpy128 (dst, src, len);	/* mose case */
	} else if (!(dstl & 1) && !(srcl & 1) && !(len & 1)) {
		smc_memcpy2 (dst, src, len);
	} else {
		smc_memcpy1 (dst, src, len);
	}
	return dst;
}

static struct sdp_smc *sdp_smc;

/* for validataion of bank pointer */
static struct sdp_smc* bank_to_smc (struct sdp_smc_bank *bank)
{
	struct sdp_smc_bank *tmp = (bank - bank->id);	
	struct sdp_smc *smc = container_of((void *)tmp, struct sdp_smc, bank);
	/* FIXME: debug */
	if (smc != sdp_smc) {
		printk (KERN_ERR "bank pointer bad? %p/%p\n", smc, bank);
		BUG_ON(1);
	}
	return smc;
}

static dma_addr_t sdp_smc_get_dmaaddr (struct sdp_smc_bank *bank, void *addr, size_t len)
{
	if ( (bank->va_base > (u32)addr) ||
		((bank->va_base + bank->size) < ((u32)addr + len))) {
		dev_err (bank_to_smc(bank)->dev, "addr translation failed %p (%x, %x, %x)\n",
			addr, bank->va_base, bank->pa_base, bank->size);
		return (dma_addr_t)0;
	}
	return (dma_addr_t)(bank->pa_base + ((u32)addr - bank->va_base));
}

static void sdp_smc_write (struct sdp_smc_bank *bank, void *dst, void *src, size_t len)
{
	smc_memcpy (dst, src, len);
}

static void sdp_smc_read (struct sdp_smc_bank *bank, void *dst, void *src, size_t len)
{
	struct sdp_smc *smc = bank_to_smc(bank);

#if defined(CONFIG_SDP_SMC_DMA)
	if (((u32)dst & 3) ||((u32)src & 3) ||(len & 0x1e00) != len ) {
		dev_dbg (smc->dev, "misaligned cpu copy %p/%p/%x\n", dst, src, len);
		smc_memcpy (dst, src, len);
	} else {
		/* dst should be normal system memory */
		dma_addr_t dma_dst, dma_src;
		dma_dst = dma_map_single (smc->dev, dst, len, DMA_FROM_DEVICE);
		if (!dma_dst) {
			dev_err (smc->dev, "failed to dma mapping of %p\n", dst);
			BUG_ON(1);	/* no way to report error to caller */
		}
		
		/* src should be smc bank area, noncacheable and no read-after-write */
		dma_src = sdp_smc_get_dmaaddr (bank, src, len);
		if (!dma_src) {
			dev_err (smc->dev, "bad smc bank address for bank %d: %p\n", bank->id, src);
			BUG_ON (1);
		}		
		
		smcdma_xfer (smc, dma_dst, dma_src, len);
		
		dma_unmap_single (smc->dev, dma_dst, len, DMA_FROM_DEVICE);		
	}
#else
	smc_memcpy (dst, src, len);
#endif
}

#if defined(SDP_SMC_NEED_LOCK)
static void sdp_smc_lock_access(struct sdp_smc *smc)
{
	spin_lock (&smc->lock);
}
static void sdp_smc_unlock_access(struct sdp_smc *smc)
{
	spin_unlock (&smc->lock);
}
#else	/* SDP_SMC_NEED_LOCK */
static void sdp_smc_lock_access(struct sdp_smc *smc) {}
static void sdp_smc_unlock_access(struct sdp_smc *smc) {}
#endif

static void sdp_smc_release(struct kref *kref)
{
	struct sdp_smc *smc = container_of (kref, struct sdp_smc, kref);
	int i;
		
	for (i=0; i<SDP_SMC_MAX_BANKS; i++) {
		struct sdp_smc_bank *bank = &smc->bank[i];
		if (bank->state == SDPSMC_STATE_INITED) {
			iounmap ((void *)bank->va_base);
			release_mem_region ((resource_size_t)bank->pa_base, bank->size);
		}		
	}
	
	iounmap (smc->dma_regs);
	release_mem_region (smc->remapped_addr, smc->remapped_len);	
	
	kfree (smc);
	sdp_smc = NULL;
}

static void sdp_smc_get(struct sdp_smc *smc)
{
	if (smc)
		kref_get (&smc->kref);
}

static void sdp_smc_put(struct sdp_smc *smc)
{
	if (smc)
		kref_put (&smc->kref, sdp_smc_release);
}

/* smc bank operations */
static void sdp_smc_read_stream(struct sdp_smc_bank *bank, void *dst, void *src, size_t len)
{
	struct sdp_smc *smc = bank_to_smc(bank);
	
	sdp_smc_lock_access (smc);
	
	sdp_smc_read (bank, dst, src, len);
	
	sdp_smc_unlock_access (smc);
}

static void sdp_smc_write_stream(struct sdp_smc_bank *bank, void *dst, void *src, size_t len)
{
	struct sdp_smc *smc = bank_to_smc(bank);
	
	sdp_smc_lock_access (smc);
		
	sdp_smc_write (bank, dst, src, len);
	
	sdp_smc_unlock_access (smc);
}

static struct sdp_smc_operations sdp_smc_ops = {
	.read_stream	= sdp_smc_read_stream,
	.write_stream	= sdp_smc_write_stream,
};

struct sdp_smc_bank* sdp_smc_getbank(int id, u32 base, u32 size)
{
	int i;
	struct sdp_smc_bank *bank = NULL;
	
	if (!sdp_smc)
		return NULL;

	spin_lock (&sdp_smc->lock);	
	sdp_smc_get (sdp_smc);
	for (i=0; i<SDP_SMC_MAX_BANKS; i++) {
		struct sdp_smc_bank *pred = &sdp_smc->bank[i];
		if (pred->state == SDPSMC_STATE_INITED && pred->pa_base == base) {
			bank = pred;
			break;
		}
	}
	spin_unlock (&sdp_smc->lock);
	
	return bank;
}

void sdp_smc_putbank(void *ptr)
{
	struct sdp_smc_bank *bank = (struct sdp_smc_bank *)ptr;
	struct sdp_smc *smc = bank_to_smc (bank);
	sdp_smc_put (smc);
}

EXPORT_SYMBOL(sdp_smc_getbank);
EXPORT_SYMBOL(sdp_smc_putbank);

static int sdp_smc_probe(struct platform_device *pdev)
{
	struct resource *res_reg = NULL;
	struct sdp_smc *smc;
	int ret = 0, i;
	
	smc = kzalloc(sizeof(*smc), GFP_KERNEL);
	if (!smc) {
		dev_err (&pdev->dev, "sdp-smc allocation failed.\n");
		return -ENOMEM;
	}
	
	kref_init (&smc->kref);
	spin_lock_init (&smc->lock);
	
	res_reg = platform_get_resource (pdev, IORESOURCE_MEM, 0);
	if (!res_reg) {
		dev_err (&pdev->dev, "sdp-smc no resource defined for this platform.\n");
		ret = -ENODEV;
		goto err0;
	}
	
	smc->remapped_addr = res_reg->start;
	smc->remapped_len = res_reg->end - res_reg->start + 1;	
	
	if (!request_mem_region(res_reg->start, smc->remapped_len, "sdp-smc")) {
		dev_err (&pdev->dev, "sdp-smc resource is used.\n");
		ret = -EBUSY;
		goto err0;
	}
	
	smc->dma_regs = ioremap (smc->remapped_addr, smc->remapped_len);
	if (!smc->dma_regs) {
		dev_err (&pdev->dev, "sdp-smc failed to remap.\n");
		ret = -EFAULT;
		goto err1;
	}
	
	dev_info (&pdev->dev, "SMCDMA register remapped: 0x%p - 0x%x.\n",
		smc->dma_regs, smc->remapped_addr);

	for (i=0; i<SDP_SMC_MAX_BANKS; i++) {
		struct sdp_smc_bank *bank = &smc->bank[i];
		struct resource *res =
			platform_get_resource (pdev, IORESOURCE_MEM, i+1);
		if (!res) {
			bank->state = SDPSMC_STATE_UNUSED;
			continue;
		}
		bank->id = i;
		bank->pa_base = res->start;
		bank->size = res->end - res->start + 1;
		if (!request_mem_region (bank->pa_base, bank->size,
						"sdp-smcbank")) {
			dev_err (&pdev->dev, "sdp-smc bank %d resource is used.\n", i);
			ret = -EBUSY;
			goto err2;
		}
		bank->va_base = (u32)ioremap (bank->pa_base, bank->size);
		if (!bank->va_base) {
			dev_err (&pdev->dev, "sdp-smc bank %d failed to remap.\n", i);
			ret = -EFAULT;
			goto err2;
		}
		bank->ops = &sdp_smc_ops;
		bank->state = SDPSMC_STATE_INITED;
		
		dev_info (&pdev->dev, "sdp-smc bank%d at 0x%x~ 0x%xbytes, remapped=0x%x.\n",
			i, bank->pa_base, bank->size, bank->va_base);
	}
	
	dev_set_drvdata (&pdev->dev, smc);
	smc->dev = &pdev->dev;
	sdp_smc = smc;
	return 0;
err2:
	for (i=0; i<SDP_SMC_MAX_BANKS; i++) {
		struct sdp_smc_bank *bank = &smc->bank[i];
		if (bank->state != SDPSMC_STATE_UNUSED) {
			if (bank->va_base)
				iounmap ((void*)bank->va_base);
			release_mem_region (bank->pa_base, bank->size);
		}
	}
err1:
	iounmap (smc->dma_regs);
	release_mem_region (res_reg->start, smc->remapped_len);
err0:
	sdp_smc_put (smc);
	sdp_smc = NULL;
	
	return ret;
}

static int sdp_smc_remove(struct platform_device *pdev)
{
	struct sdp_smc *smc = dev_get_drvdata (&pdev->dev);
	BUG_ON (smc != sdp_smc);
	sdp_smc_put (smc);	
	return 0;
}

static struct platform_driver sdp_smc_driver = {
	.probe = sdp_smc_probe,
	.remove = sdp_smc_remove,
	.driver = {
		.name = "sdp-smc",
		.bus = &platform_bus_type,
	},
};

static int __init sdp_smc_init(void)
{
	int ret;
	
	ret = platform_driver_register (&sdp_smc_driver);
	if (ret) {
		printk (KERN_ERR "Failed to register sdp-smc driver\n");
	}
	
	return ret;
}

static void __exit sdp_smc_exit(void)
{
	platform_driver_unregister (&sdp_smc_driver);
}

module_init (sdp_smc_init);
module_exit (sdp_smc_exit);

