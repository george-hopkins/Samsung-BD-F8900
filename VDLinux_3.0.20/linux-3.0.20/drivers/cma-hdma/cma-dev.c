/*
 * Contiguous Memory Allocator userspace driver
 * Copyright (c) 2010 by Samsung Electronics.
 * Written by Michal Nazarewicz (m.nazarewicz@samsung.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 */

#define pr_fmt(fmt) "cma: " fmt

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

#include "cma-dev.h"

static int  cma_file_open(struct inode *inode, struct file *file);
static int  cma_file_release(struct inode *inode, struct file *file);
static long cma_file_ioctl(struct file *file, unsigned cmd, unsigned long arg);
static int  cma_file_mmap(struct file *file, struct vm_area_struct *vma);

static const struct file_operations cma_fops = {
	.owner          = THIS_MODULE,
	.open           = cma_file_open,
	.release        = cma_file_release,
	.unlocked_ioctl = cma_file_ioctl,
	.mmap           = cma_file_mmap,
};

static struct class *cma_class;
static DEFINE_IDR(cma_idr);
static DEFINE_MUTEX(cma_idr_mutex);

static int  cma_file_open(struct inode *inode, struct file *file)
{
	struct cma_private_data *prv;
	struct cmaregion *region;
	int minor = iminor(inode);

	mutex_lock(&cma_idr_mutex);
	region = idr_find(&cma_idr, minor);
	mutex_unlock(&cma_idr_mutex);

	if (!region || !region->ctx)
		return -ENODEV;

	dev_dbg(region->dev, "%s(%p)\n", __func__, (void *)file);

	prv = kzalloc(sizeof *prv, GFP_KERNEL);
	if (!prv)
		return -ENOMEM;

	prv->region = region;
	file->private_data = prv;

	return 0;
}

static int  cma_file_release(struct inode *inode, struct file *file)
{
	struct cma_private_data *prv = file->private_data;

	dev_dbg(prv->region->dev, "%s(%p)\n", __func__, (void *)file);

	if (prv->cm) {
		cm_unpin(prv->cm);
		cm_free(prv->cm);
	}
	kfree(prv);

	return 0;
}

static long cma_file_ioctl_req(struct cma_private_data *prv, unsigned long arg)
{
	struct cma_alloc_request req;
	struct cm *cm;

	dev_dbg(prv->region->dev, "%s()\n", __func__);

	if (!arg)
		return -EINVAL;

	if (copy_from_user(&req, (void *)arg, sizeof req))
		return -EFAULT;

	if (req.magic != CMA_MAGIC)
		return -ENOTTY;

	if (prv->cm) {
		dev_dbg(prv->region->dev,
			"current file descriptor already allocated chunk\n");
		return -EBUSY;
	}

	/* May happen on 32 bit system. */
	if (req.size > ~(unsigned long)0 || req.alignment > ~(unsigned long)0)
		return -EINVAL;

	req.size = PAGE_ALIGN(req.size);
	if (req.size > ~(unsigned long)0)
		return -EINVAL;

	cm = cm_alloc(prv->region->ctx, req.size, req.alignment, NULL);
	if (IS_ERR(cm))
		return PTR_ERR(cm);

	prv->phys = cm_pin(cm);
	prv->size = req.size;
	req.start = prv->phys;
	if (copy_to_user((void *)arg, &req, sizeof req)) {
		cm_unpin(cm);
		cm_free(cm);
		return -EFAULT;
	}
	prv->cm    = cm;

	dev_dbg(prv->region->dev, "allocated %p@%p\n",
		(void *)prv->size, (void *)prv->phys);

	return 0;
}

static long cma_file_ioctl_get_free(struct cma_private_data *prv,
				unsigned long arg)
{
	if (!arg)
		return -EINVAL;

	if (copy_to_user((void *)arg, &prv->region->free,
			sizeof(unsigned long)))
		return -EFAULT;

	return 0;
}

static long cma_file_ioctl_get_size(struct cma_private_data *prv,
				unsigned long arg)
{
	if (!arg)
		return -EINVAL;

	if (copy_to_user((void *)arg, &prv->region->size,
			sizeof(unsigned long)))
		return -EFAULT;

	return 0;
}

static long cma_file_ioctl_get_largest(struct cma_private_data *prv,
				unsigned long arg)
{
	unsigned long largest = cma_get_largest(prv->region->ctx);

	if (!arg)
		return -EINVAL;

	if (copy_to_user((void *)arg, &largest, sizeof(unsigned long)))
		return -EFAULT;

	return 0;
}

static long cma_file_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	struct cma_private_data *prv = file->private_data;

	dev_dbg(prv->region->dev, "%s(%p)\n", __func__, (void *)file);

	if ((cmd == IOCTL_CMA_ALLOC ||
		cmd == IOCTL_CMA_GET_FREE ||
		cmd == IOCTL_CMA_GET_LARGEST ||
		cmd == IOCTL_CMA_GET_SIZE) != !prv->cm)
		return -EBADFD;

	switch (cmd) {
	case IOCTL_CMA_ALLOC:
		return cma_file_ioctl_req(prv, arg);

	case IOCTL_CMA_GET_FREE:
		return cma_file_ioctl_get_free(prv, arg);

	case IOCTL_CMA_GET_LARGEST:
		return cma_file_ioctl_get_largest(prv, arg);

	case IOCTL_CMA_GET_SIZE:
		return cma_file_ioctl_get_size(prv, arg);

	default:
		return -ENOTTY;
	}
}

static int  cma_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct cma_private_data *prv = file->private_data;
	unsigned long pgoff, offset, length;

	dev_dbg(prv->region->dev, "%s(%p)\n", __func__, (void *)file);

	if (!prv->cm)
		return -EBADFD;

	pgoff  = vma->vm_pgoff;
	offset = pgoff << PAGE_SHIFT;
	length = vma->vm_end - vma->vm_start;

	if (offset          >= prv->size
	 || length          >  prv->size
	 || offset + length >  prv->size)
		return -ENOSPC;

	return remap_pfn_range(vma, vma->vm_start,
			       __phys_to_pfn(prv->phys) + pgoff,
			       length, vma->vm_page_prot);
}

ssize_t cma_region_show_start(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct cmaregion *region = dev_get_drvdata(dev);

	return sprintf(buf, "0x%lx\n", region->start);
}
EXPORT_SYMBOL(cma_region_show_start);

ssize_t cma_region_show_size(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct cmaregion *region = dev_get_drvdata(dev);

	return sprintf(buf, "0x%lx\n", region->size);
}
EXPORT_SYMBOL(cma_region_show_size);

ssize_t cma_region_show_free(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct cmaregion *region = dev_get_drvdata(dev);

	return sprintf(buf, "0x%lx\n", region->free);
}
EXPORT_SYMBOL(cma_region_show_free);

ssize_t cma_region_show_chunks(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int i;
	ssize_t ret = 0;
	struct cmaregion *region = dev_get_drvdata(dev);
	struct cma *cmactx = region->ctx;
	struct cm **cm = cma_get_chunks_list(cmactx);

	if (!cm)
		return 0;

	for (i = 0; i < cmactx->chunks; i++)
		ret += sprintf(buf+ret, "0x%lx@0x%lx\n",
			cm[i]->size, cm[i]->phys);

	kfree(cm);

	return ret;
}
EXPORT_SYMBOL(cma_region_show_chunks);

ssize_t cma_region_show_largest(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct cmaregion *region = dev_get_drvdata(dev);
	struct cma *cmactx = region->ctx;
	unsigned long largest = cma_get_largest(cmactx);

	return sprintf(buf, "0x%lx\n", largest);
}

static struct device_attribute cma_attrs[] = {
	__ATTR(start, 0444, cma_region_show_start, NULL),
	__ATTR(size, 0444, cma_region_show_size, NULL),
	__ATTR(free, 0444, cma_region_show_free, NULL),
	__ATTR(largest, 0444, cma_region_show_largest, NULL),
	__ATTR(chunks, 0444, cma_region_show_chunks, NULL),
	__ATTR_NULL
};

static int add_cma_region(struct cmaregion *region)
{
	int id, ret = 0;

	ret = idr_pre_get(&cma_idr, GFP_KERNEL);
	if (!ret) {
		ret = -ENOMEM;
		goto out;
	}

	mutex_lock(&cma_idr_mutex);
	ret = idr_get_new(&cma_idr, region, &id);
	mutex_unlock(&cma_idr_mutex);
	if (ret < 0)
		goto out;

	region->id = id;
	region->dev = device_create(cma_class, NULL,
		MKDEV(CMA_CHAR_MAJOR, id), region, "cma%d", id);
	if (unlikely(IS_ERR(region->dev))) {
		ret = PTR_ERR(region->dev);
		goto out_device;
	}

	dev_set_drvdata(region->dev, region);

	dev_info(region->dev, "region added size=0x%lx start=0x%lx\n",
		region->size, region->start);

	return ret;

out_device:
	mutex_lock(&cma_idr_mutex);
	idr_remove(&cma_idr, id);
	mutex_unlock(&cma_idr_mutex);
out:
	return ret;
}

static inline void add_cma_regions(void)
{
	int i;
	for (i = 0; i < cmainfo.nr_regions; i++)
		add_cma_region(&cmainfo.region[i]);
}

static void remove_cma_region(struct cmaregion *region)
{
	if (!region)
		return;
	dev_info(region->dev, "cma region size=0x%lx start=0x%lx removed\n",
		region->size, region->start);

	mutex_lock(&cma_idr_mutex);
	idr_remove(&cma_idr, region->id);
	mutex_unlock(&cma_idr_mutex);

	device_destroy(cma_class, region->id);
}

static inline void remove_cma_regions(void)
{
	int i;
	for (i = 0; i < cmainfo.nr_regions; i++)
		remove_cma_region(&cmainfo.region[i]);
}

static int __init cma_dev_init(void)
{
	int ret = register_chrdev(CMA_CHAR_MAJOR, CMA_DEV_NAME, &cma_fops);
	if (ret)
		goto out;
	cma_class = class_create(THIS_MODULE, CMA_DEV_NAME);
	if (IS_ERR(cma_class)) {
		ret = PTR_ERR(cma_class);
		goto out_unreg_chrdev;
	}
	cma_class->dev_attrs = cma_attrs;
	add_cma_regions();
	return 0;
out_unreg_chrdev:
	unregister_chrdev(CMA_CHAR_MAJOR, CMA_DEV_NAME);
out:
	pr_debug("cma: registration failed: %d\n", ret);
	return ret;
}
module_init(cma_dev_init);

static void __exit cma_dev_exit(void)
{
	remove_cma_regions();
	class_destroy(cma_class);
	unregister_chrdev(CMA_CHAR_MAJOR, CMA_DEV_NAME);
}
module_exit(cma_dev_exit);
MODULE_LICENSE("GPL");
