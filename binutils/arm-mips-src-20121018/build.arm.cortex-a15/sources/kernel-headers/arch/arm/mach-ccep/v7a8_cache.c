/*
 *	Samsung VD DTV ARM Patch for Cortex-a8 Errata 586324
 *	Created by ij.jang, seungjun.heo, tukho.kim 20101220 
 */



#include <asm/cacheflush.h>
#include <linux/module.h>

void v7a8_flush_kern_all(void);
void v7a8_flush_user_all(void);
void v7a8_flush_user_range(unsigned long start, unsigned long end, unsigned int flags);
void v7a8_coherent_kern_range(unsigned long start, unsigned long end);
void v7a8_coherent_user_range(unsigned long start, unsigned long end);
void v7a8_flush_kern_dcache_area(void * kaddr, size_t size);
void v7a8_dma_map_area(const void *start, size_t size, int dir);
void v7a8_dma_unmap_area(const void *start, size_t size, int dir);
void v7a8_dma_flush_range(const void *start, const void *end);

#if 0
struct cpu_cache_fns {
	void (*flush_kern_all)(void);
	void (*flush_user_all)(void);
	void (*flush_user_range)(unsigned long, unsigned long, unsigned int);

	void (*coherent_kern_range)(unsigned long, unsigned long);
	void (*coherent_user_range)(unsigned long, unsigned long);
	void (*flush_kern_dcache_area)(void *, size_t);

	void (*dma_map_area)(const void *, size_t, int);
	void (*dma_unmap_area)(const void *, size_t, int);

	void (*dma_flush_range)(const void *, const void *);
};
#endif

struct cpu_cache_fns v7a8_cpu_cache = { 
	.flush_kern_all = v7a8_flush_kern_all,
	.flush_user_all = v7a8_flush_user_all,				
	.flush_user_range = v7a8_flush_user_range,
	
	.coherent_kern_range = v7a8_coherent_kern_range,    			
	.coherent_user_range = v7a8_coherent_user_range,
	.flush_kern_dcache_area = v7a8_flush_kern_dcache_area,
	
	.dma_map_area = v7a8_dma_map_area,
	.dma_unmap_area = v7a8_dma_unmap_area,
	.dma_flush_range = v7a8_dma_flush_range
};

void v7a8_flush_kern_all(void)
{
	unsigned long flags;
	u32 dummy;

	local_irq_save(flags);
	asm volatile ("ldr %0, [sp] \n" : "=r"(dummy));
	cpu_cache.flush_kern_all();
	local_irq_restore(flags);
}

void v7a8_flush_user_all(void)
{	
	unsigned long flags;
	u32 dummy;

	local_irq_save(flags);
	asm volatile ("ldr %0, [sp] \n" : "=r"(dummy));
	cpu_cache.flush_user_all();
	local_irq_restore(flags);
}

void v7a8_flush_user_range(unsigned long start, unsigned long end, unsigned int flags)
{
	unsigned long i_flags;
	volatile u32 dummy;

	local_irq_save(i_flags);
	dummy = *(volatile u32*) start;
	cpu_cache.flush_user_range(start, end, flags);
	local_irq_restore(i_flags);
}

void v7a8_coherent_kern_range(unsigned long start, unsigned long end)
{
	unsigned long flags;
	volatile u32 dummy;

	local_irq_save(flags);
	dummy = *(volatile u32*) start;
	cpu_cache.coherent_kern_range(start, end);
	local_irq_restore(flags);
}

void v7a8_coherent_user_range(unsigned long start, unsigned long end)
{
	unsigned long flags;
	volatile u32 dummy;

	local_irq_save(flags);
	dummy = *(volatile u32*) start;
	cpu_cache.coherent_user_range(start, end);
	local_irq_restore(flags);

}
void v7a8_flush_kern_dcache_area(void * kaddr, size_t size)
{
	unsigned long flags;
	volatile u32 dummy;

	local_irq_save(flags);
	dummy = *(volatile u32*) kaddr;
	cpu_cache.flush_kern_dcache_area(kaddr, size);
	local_irq_restore(flags);
}

void v7a8_dma_map_area(const void *start, size_t size, int dir)
{
	unsigned long flags;
	volatile u32 dummy;

	local_irq_save(flags);
	dummy = *(volatile u32*) start;
	cpu_cache.dma_map_area(start, size, dir);
	local_irq_restore(flags);
}

void v7a8_dma_unmap_area(const void *start, size_t size, int dir)
{
	unsigned long flags;
	volatile u32 dummy;

	local_irq_save(flags);
	dummy = *(volatile u32*) start;
	cpu_cache.dma_unmap_area(start, size, dir);
	local_irq_restore(flags);
}

void v7a8_dma_flush_range(const void *start, const void *end)
{
	unsigned long flags;
	volatile u32 dummy;

	local_irq_save(flags);
	dummy = *(volatile u32*) start;
	cpu_cache.dma_flush_range(start, end);
	local_irq_restore(flags);
}

EXPORT_SYMBOL(v7a8_cpu_cache);

