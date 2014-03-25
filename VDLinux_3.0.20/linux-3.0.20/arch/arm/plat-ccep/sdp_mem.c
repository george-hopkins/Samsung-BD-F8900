/*********************************************************************************************
 *
 *	sdp_mem.c (Samsung Soc HW memory allocation)
 *
 *	author : seungjun.heo@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
 * Date 	author		Description
 * ----------------------------------------------------------------------------------------
// May,18,2011 	seungjun.heo	created
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

#include <asm/memory.h>		// alloc_page_node
#include <asm/uaccess.h>	// copy_from_user
#include <asm-generic/mman.h>	// copy_from_user
#include <asm/cacheflush.h>	// copy_from_user

#define DEBUG_SDP_MEM

#ifdef DEBUG_SDP_MEM
#define DPRINT_SDP_MEM(fmt, args...) printk("[%s]" fmt, __FUNCTION__, ##args)
#else
#define DPRINT_SDP_MEM(fmt, args...) 
#endif 

#define PRINT_MEM_ERR(fmt, args...) printk(KERN_ERR"[%s]" fmt, __FUNCTION__, ##args)

#define DRV_NAME		"sdp_mem"
#define SDP_MEM_MINOR		193

static const struct vm_operations_struct mmap_mem_ops = {
	.open = NULL,
#ifdef CONFIG_HAVE_IOREMAP_PROT
	.access = generic_access_phys
#endif
};

static int
sdp_mem_mmap(struct file * file, struct vm_area_struct * vma)
{
	size_t size = vma->vm_end - vma->vm_start;

	if (file->f_flags & O_SYNC)
		vma->vm_page_prot = __pgprot_modify(vma->vm_page_prot, L_PTE_MT_MASK, L_PTE_MT_UNCACHED | L_PTE_XN);

	vma->vm_ops = &mmap_mem_ops;

   /* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma,
    					vma->vm_start,
                        vma->vm_pgoff,
                        size,
                        vma->vm_page_prot))
                return -EAGAIN;
    return 0;
}


static int sdp_mem_open(struct inode *inode, struct file *file)
{
	DPRINT_SDP_MEM("open\n");

	return 0;
}


static int sdp_mem_release (struct inode *inode, struct file *file)
{
	DPRINT_SDP_MEM("release\n");

	return 0;
}

static const struct file_operations sdp_mem_fops = {
	.owner = THIS_MODULE,
	.open  = sdp_mem_open,
	.release = sdp_mem_release,
	.mmap = sdp_mem_mmap,
};

static struct miscdevice sdp_mem_dev = {
	.minor = SDP_MEM_MINOR,
	.name = DRV_NAME,
	.fops = &sdp_mem_fops	
};


static int __init sdp_mem_init(void)
{
	int ret_val = 0;

	ret_val = misc_register(&sdp_mem_dev);

	if(ret_val){
		printk(KERN_ERR "[ERR]%s: misc register failed\n", DRV_NAME);
	}
	else {
		printk(KERN_INFO"[SDP_MEM] %s initialized\n", DRV_NAME);
	}

	return ret_val;
}

static void __exit sdp_mem_exit(void)
{

	misc_deregister(&sdp_mem_dev);

	return;
}

module_init(sdp_mem_init);
module_exit(sdp_mem_exit);

MODULE_AUTHOR("seungjun.heo@samsung.com");
MODULE_DESCRIPTION("Driver for SDP HW memory allocation");
MODULE_LICENSE("Proprietary");
