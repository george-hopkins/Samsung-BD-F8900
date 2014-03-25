/*
 * Hardware Dedicated Memory Allocator userspace driver
 * Copyright (c) 2011 by Samsung Electronics.
 * Written by Vasily Leonenko (v.leonenko@samsung.com)
 */

#define pr_fmt(fmt) "hdma: " fmt

#ifdef CONFIG_CMA_DEBUG
#  define DEBUG
#endif

#include <linux/errno.h>       /* Error numbers */
#include <linux/err.h>         /* IS_ERR_VALUE() */
#include <linux/fs.h>          /* struct file */
#include <linux/mm.h>          /* Memory stuff */
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/module.h>      /* Standard module stuff */
#include <linux/device.h>      /* struct device, dev_dbg() */
#include <linux/types.h>       /* Just to be safe ;) */
#include <linux/uaccess.h>     /* __copy_{to,from}_user */
#include <linux/miscdevice.h>  /* misc_register() and company */
#include <linux/idr.h>         /* idr functions */

#include <linux/cma.h>
#include <linux/hdma.h>

#include "cma-dev.h"
#include "hdma-dev.h"

#define HDMA_DEV_NAME "hdma"

static long hdma_file_ioctl(struct file *file, unsigned cmd, unsigned long arg);
static int  hdma_file_open(struct inode *inode, struct file *file);

static const struct file_operations hdma_fops = {
	.owner          = THIS_MODULE,
	.open           = hdma_file_open,
	.unlocked_ioctl = hdma_file_ioctl,
};

static struct class *hdma_class;
static DEFINE_IDR(hdma_idr);
static DEFINE_MUTEX(hdma_idr_mutex);

static struct cm	**cm;

static struct {
       enum hdma_state cur_state;
       struct mutex lock_mutex;
} hdma_info = {
       .cur_state = HDMA_IS_OFF,
       .lock_mutex = __MUTEX_INITIALIZER(hdma_info.lock_mutex),
};

enum hdma_state get_hdma_status(void)
{
       return hdma_info.cur_state;
}

int set_hdma_status(enum hdma_state state)
{
       if (state > HDMA_IS_MIGRATING)
               return -EINVAL;

       mutex_lock(&hdma_info.lock_mutex);
       hdma_info.cur_state = state;
       mutex_unlock(&hdma_info.lock_mutex);    

       return 0;
}

static int hdma_file_open(struct inode *inode, struct file *file)
{
	struct cmaregion *region;
	int minor = iminor(inode);

	mutex_lock(&hdma_idr_mutex);
	region = idr_find(&hdma_idr, minor);
	mutex_unlock(&hdma_idr_mutex);

	if (!region || !region->ctx)
		return -ENODEV;

	dev_dbg(region->dev, "%s(%p)\n", __func__, (void *)file);

	file->private_data = region;

	return 0;
}

static int alloc_hdma_region(struct cmaregion *region,
				unsigned long *failed_pages)
{
	int ret = 0;
	if (!region->ctx)
		return -EBUSY;
	if (cm[region->id])
		return 0;
	cm[region->id] = cm_alloc(region->ctx, region->size, 0, failed_pages);
	if (IS_ERR(cm[region->id])) {
		ret = PTR_ERR(cm[region->id]);
		cm[region->id] = NULL;
	} else
		cm_pin(cm[region->id]);
	return ret;
}


static int alloc_hdma_regions(unsigned long *failed_size)
{
	int i, ret = 0;
	unsigned long failed_pages = 0;
	for (i = 0; i < hdmainfo.nr_regions; i++) {
		ret = alloc_hdma_region(&hdmainfo.region[i], &failed_pages);
		if (ret) {
			if (ret == -ENOMEM) {
				if (failed_size != NULL)
					*failed_size += failed_pages;
				i++;
				break;
			} else
				goto out;
		}
	}
	if (failed_size != NULL)
		for (; i < hdmainfo.nr_regions; i++)
			*failed_size += hdmainfo.region[i].size >> PAGE_SHIFT;
out:
	return ret;
}

static void free_hdma_region(struct cmaregion *region)
{
	if (cm[region->id] == NULL || !region->ctx)
		return;
	cm_unpin(cm[region->id]);
	cm_free(cm[region->id]);
	cm[region->id] = NULL;
}

static void free_hdma_regions(void)
{
	int i;
	for (i = 0; i < hdmainfo.nr_regions; i++)
		free_hdma_region(&hdmainfo.region[i]);
}

static unsigned long hdma_regions_size(void)
{
	int i;
	unsigned long size = 0;
	for (i = 0; i < hdmainfo.nr_regions; i++)
		size += hdmainfo.region[i].size;
	return size;
}

#ifdef CONFIG_ZRAM
extern int show_zram_info(void);
#endif
struct mem_cgroup;
extern void dump_tasks_plus(const struct mem_cgroup *mem, struct seq_file *s);

DEFINE_MUTEX(hdma_ioctl_mutex);

static long hdma_file_ioctl_notify(unsigned long arg)
{
	int ret = 0;
	unsigned long failed_size = 0;

	if (hdmainfo.nr_regions == 0)
		return -ENOMEM;

	mutex_lock(&hdma_ioctl_mutex);

	switch (arg) {
	case 0: /* notify-free */
		free_hdma_regions();
		ret = hdma_regions_size() >> PAGE_SHIFT;
		break;
	case 1: /* notify-alloc */
		if (set_hdma_status(HDMA_IS_MIGRATING)) {
			printk("VDLinux HDMA msg, set_hdma_status(HDMA_IS_MIGRATING) return fail..., %s, %d\n", __func__, __LINE__);
			ret = -ENOTTY;
		}

		printk("VDLinux HDMA msg, HDMA_IS_MIGRATING.\n");
		dump_tasks_plus(NULL, NULL);
		show_free_areas(0);
#ifdef CONFIG_ZRAM
		show_zram_info();
#endif
try_again:
		ret = alloc_hdma_regions(&failed_size);
		if (ret == -ENOMEM)
			ret = failed_size;
		else if (ret) {
#ifdef CONFIG_CMA_DEBUG
			printk("try_again.. %s(failed_size : %ld)\n",__func__,failed_size);
#endif
			ret = -EAGAIN;
			goto try_again;
		}
		break;
	default:
		ret = -ENOTTY;
	}

	if(arg==0) {
		if (set_hdma_status(HDMA_IS_ON)) {
			printk("VDLinux HDMA msg, set_hdma_status(HDMA_IS_ON) return fail..., %s, %d\n", __func__, __LINE__);
			ret = -ENOTTY;
		}

		printk("VDLinux HDMA msg, HDMA_IS_ON.\n");
		show_free_areas(0);
#ifdef CONFIG_ZRAM
		show_zram_info();
#endif
	}
	if(ret==0 && arg==1) {
		if (set_hdma_status(HDMA_IS_OFF)) {
			printk("VDLinux HDMA msg, set_hdma_status(HDMA_IS_OFF) return fail..., %s, %d\n", __func__, __LINE__);
			ret = -ENOTTY;
		}

		printk("VDLinux HDMA msg, HDMA_IS_OFF.\n");
		show_free_areas(0);
#ifdef CONFIG_ZRAM
		show_zram_info();
#endif
	}

	mutex_unlock(&hdma_ioctl_mutex);

	return ret;
}

static long hdma_file_ioctl_notify_region(struct cmaregion *region,
						unsigned long arg)
{
	int ret = 0;
	unsigned long failed_pages = 0;
	if (hdmainfo.nr_regions == 0)
		return -ENOMEM;

	switch (arg) {
	case 0: /* notify-free */
		free_hdma_region(region);
		ret = region->size >> PAGE_SHIFT;
		break;
	case 1: /* notify-alloc */
		ret = alloc_hdma_region(region, &failed_pages);
		if (ret == -ENOMEM)
			ret = failed_pages;
		else if (ret)
			ret = -EAGAIN;
		break;
	default:
		ret = -ENOTTY;
	}
	return ret;
}

static long hdma_file_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	struct cmaregion *region = file->private_data;

	if (!region)
		return -EBADFD;

	switch (cmd) {
	case IOCTL_HDMA_NOTIFY_ALL:
		return hdma_file_ioctl_notify(arg);

	case IOCTL_HDMA_NOTIFY:
		return hdma_file_ioctl_notify_region(region, arg);

	default:
		return -ENOTTY;
	}
}

static struct device_attribute hdma_attrs[] = {
	__ATTR(start, 0444, cma_region_show_start, NULL),
	__ATTR(size, 0444, cma_region_show_size, NULL),
	__ATTR(free, 0444, cma_region_show_free, NULL),
	__ATTR(largest, 0444, cma_region_show_largest, NULL),
	__ATTR(chunks, 0444, cma_region_show_chunks, NULL),
	__ATTR_NULL
};

static int add_hdma_region(struct cmaregion *region)
{
	int id, ret = 0;

	ret = idr_pre_get(&hdma_idr, GFP_KERNEL);
	if (!ret) {
		ret = -ENOMEM;
		goto out;
	}

	mutex_lock(&hdma_idr_mutex);
	ret = idr_get_new(&hdma_idr, region, &id);
	mutex_unlock(&hdma_idr_mutex);
	if (ret < 0)
		goto out;

	region->id = id;
	region->dev = device_create(hdma_class, NULL,
			MKDEV(HDMA_CHAR_MAJOR, id), region, "hdma%d", id);
	if (unlikely(IS_ERR(region->dev))) {
		ret = PTR_ERR(region->dev);
		goto out_device;
	}

	dev_set_drvdata(region->dev, region);

	dev_info(region->dev, "region added size=0x%lx start=0x%lx\n",
		region->size, region->start);

	return ret;

out_device:
	mutex_lock(&hdma_idr_mutex);
	idr_remove(&hdma_idr, id);
	mutex_unlock(&hdma_idr_mutex);
out:
	return ret;
}

static inline void add_hdma_regions(void)
{
	int i;
	cm = kmalloc(sizeof(struct cm *) * hdmainfo.nr_regions, GFP_KERNEL);
	for (i = 0; i < hdmainfo.nr_regions; i++) {
		cm[i] = NULL;
		add_hdma_region(&hdmainfo.region[i]);
	}
}

static void remove_hdma_region(struct cmaregion *region)
{
	if (!region)
		return;
	dev_info(region->dev, "hdma region size=0x%lx start=0x%lx removed\n",
		region->size, region->start);

	mutex_lock(&hdma_idr_mutex);
	idr_remove(&hdma_idr, region->id);
	mutex_unlock(&hdma_idr_mutex);

	device_destroy(hdma_class, region->id);
}

static inline void remove_hdma_regions(void)
{
	int i;
	for (i = 0; i < hdmainfo.nr_regions; i++)
		remove_hdma_region(&hdmainfo.region[i]);
	kfree(cm);
}

static int __init hdma_dev_init(void)
{
	int ret = register_chrdev(HDMA_CHAR_MAJOR, HDMA_DEV_NAME, &hdma_fops);
	if (ret)
		goto out;
	hdma_class = class_create(THIS_MODULE, HDMA_DEV_NAME);
	if (IS_ERR(hdma_class)) {
		ret = PTR_ERR(hdma_class);
		goto out_unreg_chrdev;
	}
	hdma_class->dev_attrs = hdma_attrs;
	add_hdma_regions();
	alloc_hdma_regions(NULL);
	return 0;
out_unreg_chrdev:
	unregister_chrdev(HDMA_CHAR_MAJOR, HDMA_DEV_NAME);
out:
	pr_debug("hdma: registration failed: %d\n", ret);
	return ret;
}
module_init(hdma_dev_init);

static void __exit hdma_dev_exit(void)
{
	remove_hdma_regions();
	class_destroy(hdma_class);
	unregister_chrdev(HDMA_CHAR_MAJOR, HDMA_DEV_NAME);
}
module_exit(hdma_dev_exit);
MODULE_LICENSE("GPL");
