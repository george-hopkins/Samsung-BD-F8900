/*********************************************************************************************
 *
 *	sdp_hwmem.c (Samsung Soc DMA memory allocation)
 *
 *	author : tukho.kim@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
 * Date 	author		Description
 * ----------------------------------------------------------------------------------------
// Jul,09,2010 	tukho.kim	created
 ********************************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/gfp.h>		
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/proc_fs.h>

#include <asm/memory.h>		// alloc_page_node
#include <asm/uaccess.h>	// copy_from_user
#include <asm-generic/mman.h>	// copy_from_user
#include <asm/cacheflush.h>	// copy_from_user

#include <plat/sdp_hwmem_io.h>

#define DEBUG_SDP_HWMEM

#ifdef DEBUG_SDP_HWMEM
#define DPRINT_SDP_HWMEM(fmt, args...) printk("[%s]" fmt, __FUNCTION__, ##args)
#else
#define DPRINT_SDP_HWMEM(fmt, args...) 
#endif 

#define PRINT_HWMEM_ERR(fmt, args...) printk(KERN_ERR"[%s]" fmt, __FUNCTION__, ##args)

#define DRV_NAME		"sdp_hwmem"
#define SDP_HWMEM_MINOR		192

extern int sdp_get_revision_id(void);
static struct proc_dir_entry *sdpver;


int
sdp_hwmem_get(struct file * file, SDP_GET_HWMEM_T* p_hwmem_info)
{
	int	ret_val = 0;
	SDP_GET_HWMEM_T hwmem_info;

	struct page *page;
	unsigned long order, addr;
	size_t size;

	ret_val = copy_from_user(&hwmem_info, p_hwmem_info, sizeof(SDP_GET_HWMEM_T));

	if(ret_val){
		PRINT_HWMEM_ERR("get hwmem info failed\n");
		return -EINVAL;
	}
	
	DPRINT_SDP_HWMEM("size: %d, %s\n", hwmem_info.size, (hwmem_info.uncached)?"uncached":"uncached");

	file->private_data = (void*)hwmem_info.uncached;

	size = PAGE_ALIGN(hwmem_info.size);
	order = get_order(size);

	page = (hwmem_info.node < 0) ?
			alloc_pages(GFP_ATOMIC | GFP_DMA ,order):
			alloc_pages_node(hwmem_info.node, GFP_ATOMIC | GFP_DMA ,order);

	if(!page) {
		PRINT_HWMEM_ERR("alloc page failed\n");
		return -ENOMEM;
	}

	DPRINT_SDP_HWMEM("get pages: 0x%08x\n",(u32)page);

	hwmem_info.phy_addr = addr = page_to_phys(page);
	hwmem_info.vir_addr = do_mmap(file, 0, size, PROT_WRITE | PROT_READ, MAP_SHARED, addr);
	
	DPRINT_SDP_HWMEM("phy: 0x%08x, vir: 0x%08x\n",(u32)hwmem_info.phy_addr,(u32)hwmem_info.vir_addr);

	ret_val = copy_to_user((void*)p_hwmem_info, (void*)&hwmem_info, sizeof(SDP_GET_HWMEM_T));

	return 0;
}

int 
sdp_hwmem_rel(struct file* file, SDP_REL_HWMEM_T* p_hwmem_info)
{
	int	ret_val = 0;

	SDP_REL_HWMEM_T hwmem_info;

	struct mm_struct *mm = current->mm;
	unsigned long order;
	size_t 	size;

	ret_val = copy_from_user(&hwmem_info, p_hwmem_info, sizeof(SDP_REL_HWMEM_T));

	if(ret_val){
		PRINT_HWMEM_ERR("get hwmem info failed\n");
		return -EINVAL;
	}

	size = PAGE_ALIGN(hwmem_info.size);

	do_munmap(mm, hwmem_info.vir_addr, size);
	order = get_order(size);
	__free_pages(pfn_to_page(__phys_to_pfn(hwmem_info.phy_addr)), order);
	
	return 0;
}

#if defined(CONFIG_CACHE_L2X0)
extern void l2x0_flush_all(void);
#endif

static inline void sdp_hwmem_flush_all(void)
{
	unsigned long flag;

	raw_local_irq_save (flag);

	flush_cache_all ();

#ifdef CONFIG_SMP
	raw_local_irq_restore (flag);

	smp_call_function(__cpuc_flush_kern_all, NULL, 1);

	raw_local_irq_save (flag);
#endif

#if defined(CONFIG_CACHE_L2X0)
	l2x0_flush_all(); 
#endif
	raw_local_irq_restore (flag);
}

static inline void 
sdp_hwmem_inv_range(const void* vir_addr, const unsigned long phy_addr, const size_t size)
{
	dmac_map_area (vir_addr, size, DMA_FROM_DEVICE);
	outer_inv_range(phy_addr, (unsigned long)(phy_addr+size));
}

static inline void 
sdp_hwmem_clean_range(const void* vir_addr, const unsigned long phy_addr, const size_t size)
{
	dmac_map_area (vir_addr, size, DMA_TO_DEVICE);
	outer_clean_range(phy_addr, (unsigned long)(phy_addr+size));
}

static inline void 
sdp_hwmem_flush_range(const void* vir_addr, const unsigned long phy_addr, const size_t size)
{
	dmac_flush_range(vir_addr,(const void *)(vir_addr+size));
	outer_flush_range(phy_addr, (unsigned long)(phy_addr+size));
}

static const struct vm_operations_struct mmap_mem_ops = {
#ifdef CONFIG_HAVE_IOREMAP_PROT
	.access = generic_access_phys
#endif
};

static int
sdp_hwmem_mmap(struct file * file, struct vm_area_struct * vma)
{
	size_t size = vma->vm_end - vma->vm_start;


	if (file->f_flags & O_SYNC)
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	vma->vm_ops = &mmap_mem_ops;

   /* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma,
    			vma->vm_start,
                        vma->vm_pgoff,
                        size,
                        vma->vm_page_prot)) {
        	return -EAGAIN;
	}
    return 0;
}

int 
sdp_hwmem_cache(unsigned int cmd, SDP_CACHE_HWMEM_T * p_hwmem_info)
{
	int ret_val = 0;
	SDP_CACHE_HWMEM_T hwmem_info;

	ret_val = copy_from_user(&hwmem_info, p_hwmem_info, sizeof(SDP_CACHE_HWMEM_T));

	if(ret_val){
		PRINT_HWMEM_ERR("get hwmem info failed\n");
		return -EINVAL;
	}

	switch(cmd) {
		case (CMD_HWMEM_INV_RANGE): 
			sdp_hwmem_inv_range((const void *)hwmem_info.vir_addr, 
								(const unsigned long)hwmem_info.phy_addr, 
								hwmem_info.size);
			break;
		case (CMD_HWMEM_CLEAN_RANGE): 
			sdp_hwmem_clean_range((const void *)hwmem_info.vir_addr, 
								(const unsigned long)hwmem_info.phy_addr, 
								hwmem_info.size);
			break;
		case (CMD_HWMEM_FLUSH_RANGE): 
			sdp_hwmem_flush_range((const void *)hwmem_info.vir_addr, 
								(const unsigned long)hwmem_info.phy_addr, 
								hwmem_info.size);
			break;
		default:
			break;
	}

	return 0;
}



static int 
sdp_hwmem_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long args)
{
	int ret_val = 0;

	switch(cmd){
		case (CMD_HWMEM_GET): 
			ret_val = sdp_hwmem_get(file, (SDP_GET_HWMEM_T*) args);
			break;
		case (CMD_HWMEM_RELEASE): 
			ret_val = sdp_hwmem_rel(file, (SDP_REL_HWMEM_T*) args);
			break;
		case (CMD_HWMEM_CLEAN): 
			break;
		case (CMD_HWMEM_FLUSH): 
			sdp_hwmem_flush_all();
			break;
		case (CMD_HWMEM_INV_RANGE): 
		case (CMD_HWMEM_CLEAN_RANGE): 
		case (CMD_HWMEM_FLUSH_RANGE): 
			ret_val = sdp_hwmem_cache(cmd, (SDP_CACHE_HWMEM_T *) args);
			break;
		case (CMD_HWMEM_GET_REVISION_ID):
			{
				unsigned int u32Val = sdp_get_revision_id();
				ret_val = copy_to_user((void __user *) args, &u32Val, sizeof(u32Val));
			}
		default:
			break;
	}	

	return ret_val;
}

static int sdp_hwmem_open(struct inode *inode, struct file *file)
{
	DPRINT_SDP_HWMEM("open\n");


	return 0;
}


static int sdp_hwmem_release (struct inode *inode, struct file *file)
{

	DPRINT_SDP_HWMEM("release\n");

	return 0;
}

static const struct file_operations sdp_hwmem_fops = {
	.owner = THIS_MODULE,
	.open  = sdp_hwmem_open,
	.release = sdp_hwmem_release,
	.unlocked_ioctl = sdp_hwmem_ioctl,
	.mmap = sdp_hwmem_mmap,
};

static struct miscdevice sdp_hwmem_dev = {
	SDP_HWMEM_MINOR,
	DRV_NAME,
	&sdp_hwmem_fops	
};

static int proc_read_sdpver(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	printk("ES%d\n", sdp_get_revision_id());
	return 0;
}

static int __init sdp_hwmem_init(void)
{
	int ret_val = 0;

	ret_val = misc_register(&sdp_hwmem_dev);

	if(ret_val){
		printk(KERN_ERR "[ERR]%s: misc register failed\n", DRV_NAME);
	}
	else {
		printk(KERN_INFO"[SDP_HWMEM] %s initialized\n", DRV_NAME);
	}

	sdpver = create_proc_entry("sdp_version", 0644, NULL);
	if(sdpver == NULL)
	{
		printk(KERN_ERR "[SDP_HWMEM] fail to create proc sdpver info\n");
	}
	else
	{
		sdpver->read_proc = proc_read_sdpver;
		printk("/proc/sdp_version is registered!\n");
	}

	return ret_val;
}

static void __exit sdp_hwmem_exit(void)
{

	misc_deregister(&sdp_hwmem_dev);

	return;
}

module_init(sdp_hwmem_init);
module_exit(sdp_hwmem_exit);

MODULE_AUTHOR("tukho.kim@samsung.com");
MODULE_DESCRIPTION("Driver for SDP DMA memory allocation");
MODULE_LICENSE("Proprietary");
