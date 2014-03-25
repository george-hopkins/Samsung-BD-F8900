/*
 *  gendisk handling
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/kobj_map.h>
#include <linux/buffer_head.h>
#include <linux/mutex.h>

#ifdef CONFIG_KDEBUGD_COUNTER_MONITOR
#include <kdebugd/kdebugd.h>
#include <linux/time.h>
#include <kdebugd/sec_diskusage.h>
#endif

#include <linux/idr.h>
#include <linux/log2.h>

#include "blk.h"

#ifdef CONFIG_FS_SEL_READAHEAD
extern atomic_t *create_readahead_proc(const char *name);
extern void remove_readahead_proc(const char *name);
extern atomic_t *get_readahead_entry(const char *name);
#endif

static DEFINE_MUTEX(block_class_lock);
struct kobject *block_depr;

/* for extended dynamic devt allocation, currently only one major is used */
#define MAX_EXT_DEVT		(1 << MINORBITS)

/* For extended devt allocation.  ext_devt_mutex prevents look up
 * results from going away underneath its user.
 */
static DEFINE_MUTEX(ext_devt_mutex);
static DEFINE_IDR(ext_devt_idr);

static struct device_type disk_type;

static void disk_alloc_events(struct gendisk *disk);
static void disk_add_events(struct gendisk *disk);
static void disk_del_events(struct gendisk *disk);
static void disk_release_events(struct gendisk *disk);

/**
 * disk_get_part - get partition
 * @disk: disk to look partition from
 * @partno: partition number
 *
 * Look for partition @partno from @disk.  If found, increment
 * reference count and return it.
 *
 * CONTEXT:
 * Don't care.
 *
 * RETURNS:
 * Pointer to the found partition on success, NULL if not found.
 */
struct hd_struct *disk_get_part(struct gendisk *disk, int partno)
{
	struct hd_struct *part = NULL;
	struct disk_part_tbl *ptbl;

	if (unlikely(partno < 0))
		return NULL;

	rcu_read_lock();

	ptbl = rcu_dereference(disk->part_tbl);
	if (likely(partno < ptbl->len)) {
		part = rcu_dereference(ptbl->part[partno]);
		if (part)
			get_device(part_to_dev(part));
	}

	rcu_read_unlock();

	return part;
}
EXPORT_SYMBOL_GPL(disk_get_part);

/**
 * disk_part_iter_init - initialize partition iterator
 * @piter: iterator to initialize
 * @disk: disk to iterate over
 * @flags: DISK_PITER_* flags
 *
 * Initialize @piter so that it iterates over partitions of @disk.
 *
 * CONTEXT:
 * Don't care.
 */
void disk_part_iter_init(struct disk_part_iter *piter, struct gendisk *disk,
			  unsigned int flags)
{
	struct disk_part_tbl *ptbl;

	rcu_read_lock();
	ptbl = rcu_dereference(disk->part_tbl);

	piter->disk = disk;
	piter->part = NULL;

	if (flags & DISK_PITER_REVERSE)
		piter->idx = ptbl->len - 1;
	else if (flags & (DISK_PITER_INCL_PART0 | DISK_PITER_INCL_EMPTY_PART0))
		piter->idx = 0;
	else
		piter->idx = 1;

	piter->flags = flags;

	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(disk_part_iter_init);

/**
 * disk_part_iter_next - proceed iterator to the next partition and return it
 * @piter: iterator of interest
 *
 * Proceed @piter to the next partition and return it.
 *
 * CONTEXT:
 * Don't care.
 */
struct hd_struct *disk_part_iter_next(struct disk_part_iter *piter)
{
	struct disk_part_tbl *ptbl;
	int inc, end;

	/* put the last partition */
	disk_put_part(piter->part);
	piter->part = NULL;

	/* get part_tbl */
	rcu_read_lock();
	ptbl = rcu_dereference(piter->disk->part_tbl);

	/* determine iteration parameters */
	if (piter->flags & DISK_PITER_REVERSE) {
		inc = -1;
		if (piter->flags & (DISK_PITER_INCL_PART0 |
				    DISK_PITER_INCL_EMPTY_PART0))
			end = -1;
		else
			end = 0;
	} else {
		inc = 1;
		end = ptbl->len;
	}

	/* iterate to the next partition */
	for (; piter->idx != end; piter->idx += inc) {
		struct hd_struct *part;

		part = rcu_dereference(ptbl->part[piter->idx]);
		if (!part)
			continue;
		if (!part->nr_sects &&
		    !(piter->flags & DISK_PITER_INCL_EMPTY) &&
		    !(piter->flags & DISK_PITER_INCL_EMPTY_PART0 &&
		      piter->idx == 0))
			continue;

		get_device(part_to_dev(part));
		piter->part = part;
		piter->idx += inc;
		break;
	}

	rcu_read_unlock();

	return piter->part;
}
EXPORT_SYMBOL_GPL(disk_part_iter_next);

/**
 * disk_part_iter_exit - finish up partition iteration
 * @piter: iter of interest
 *
 * Called when iteration is over.  Cleans up @piter.
 *
 * CONTEXT:
 * Don't care.
 */
void disk_part_iter_exit(struct disk_part_iter *piter)
{
	disk_put_part(piter->part);
	piter->part = NULL;
}
EXPORT_SYMBOL_GPL(disk_part_iter_exit);

static inline int sector_in_part(struct hd_struct *part, sector_t sector)
{
	return part->start_sect <= sector &&
		sector < part->start_sect + part->nr_sects;
}

/**
 * disk_map_sector_rcu - map sector to partition
 * @disk: gendisk of interest
 * @sector: sector to map
 *
 * Find out which partition @sector maps to on @disk.  This is
 * primarily used for stats accounting.
 *
 * CONTEXT:
 * RCU read locked.  The returned partition pointer is valid only
 * while preemption is disabled.
 *
 * RETURNS:
 * Found partition on success, part0 is returned if no partition matches
 */
struct hd_struct *disk_map_sector_rcu(struct gendisk *disk, sector_t sector)
{
	struct disk_part_tbl *ptbl;
	struct hd_struct *part;
	int i;

	ptbl = rcu_dereference(disk->part_tbl);

	part = rcu_dereference(ptbl->last_lookup);
	if (part && sector_in_part(part, sector))
		return part;

	for (i = 1; i < ptbl->len; i++) {
		part = rcu_dereference(ptbl->part[i]);

		if (part && sector_in_part(part, sector)) {
			rcu_assign_pointer(ptbl->last_lookup, part);
			return part;
		}
	}
	return &disk->part0;
}
EXPORT_SYMBOL_GPL(disk_map_sector_rcu);

/*
 * Can be deleted altogether. Later.
 *
 */
static struct blk_major_name {
	struct blk_major_name *next;
	int major;
	char name[16];
} *major_names[BLKDEV_MAJOR_HASH_SIZE];


#ifdef CONFIG_BOOTPROFILE
static inline void sec_bp_start(void);
#endif /* CONFIG_BOOTPROFILE */

/* index in the above - for now: assume no multimajor ranges */
static inline int major_to_index(unsigned major)
{
	return major % BLKDEV_MAJOR_HASH_SIZE;
}

#ifdef CONFIG_PROC_FS
void blkdev_show(struct seq_file *seqf, off_t offset)
{
	struct blk_major_name *dp;

	if (offset < BLKDEV_MAJOR_HASH_SIZE) {
		mutex_lock(&block_class_lock);
		for (dp = major_names[offset]; dp; dp = dp->next)
			seq_printf(seqf, "%3d %s\n", dp->major, dp->name);
		mutex_unlock(&block_class_lock);
	}
}
#endif /* CONFIG_PROC_FS */

/**
 * register_blkdev - register a new block device
 *
 * @major: the requested major device number [1..255]. If @major=0, try to
 *         allocate any unused major number.
 * @name: the name of the new block device as a zero terminated string
 *
 * The @name must be unique within the system.
 *
 * The return value depends on the @major input parameter.
 *  - if a major device number was requested in range [1..255] then the
 *    function returns zero on success, or a negative error code
 *  - if any unused major number was requested with @major=0 parameter
 *    then the return value is the allocated major number in range
 *    [1..255] or a negative error code otherwise
 */
int register_blkdev(unsigned int major, const char *name)
{
	struct blk_major_name **n, *p;
	int index, ret = 0;

	mutex_lock(&block_class_lock);

	/* temporary */
	if (major == 0) {
		for (index = ARRAY_SIZE(major_names)-1; index > 0; index--) {
			if (major_names[index] == NULL)
				break;
		}

		if (index == 0) {
			printk("register_blkdev: failed to get major for %s\n",
			       name);
			ret = -EBUSY;
			goto out;
		}
		major = index;
		ret = major;
	}

	p = kmalloc(sizeof(struct blk_major_name), GFP_KERNEL);
	if (p == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	p->major = major;
	strlcpy(p->name, name, sizeof(p->name));
	p->next = NULL;
	index = major_to_index(major);

	for (n = &major_names[index]; *n; n = &(*n)->next) {
		if ((*n)->major == major)
			break;
	}
	if (!*n)
		*n = p;
	else
		ret = -EBUSY;

	if (ret < 0) {
		printk("register_blkdev: cannot get major %d for %s\n",
		       major, name);
		kfree(p);
	}
out:
	mutex_unlock(&block_class_lock);
	return ret;
}

EXPORT_SYMBOL(register_blkdev);

void unregister_blkdev(unsigned int major, const char *name)
{
	struct blk_major_name **n;
	struct blk_major_name *p = NULL;
	int index = major_to_index(major);

	mutex_lock(&block_class_lock);
	for (n = &major_names[index]; *n; n = &(*n)->next)
		if ((*n)->major == major)
			break;
	if (!*n || strcmp((*n)->name, name)) {
		WARN_ON(1);
	} else {
		p = *n;
		*n = p->next;
	}
	mutex_unlock(&block_class_lock);
	kfree(p);
}

EXPORT_SYMBOL(unregister_blkdev);

static struct kobj_map *bdev_map;

/**
 * blk_mangle_minor - scatter minor numbers apart
 * @minor: minor number to mangle
 *
 * Scatter consecutively allocated @minor number apart if MANGLE_DEVT
 * is enabled.  Mangling twice gives the original value.
 *
 * RETURNS:
 * Mangled value.
 *
 * CONTEXT:
 * Don't care.
 */
static int blk_mangle_minor(int minor)
{
#ifdef CONFIG_DEBUG_BLOCK_EXT_DEVT
	int i;

	for (i = 0; i < MINORBITS / 2; i++) {
		int low = minor & (1 << i);
		int high = minor & (1 << (MINORBITS - 1 - i));
		int distance = MINORBITS - 1 - 2 * i;

		minor ^= low | high;	/* clear both bits */
		low <<= distance;	/* swap the positions */
		high >>= distance;
		minor |= low | high;	/* and set */
	}
#endif
	return minor;
}

/**
 * blk_alloc_devt - allocate a dev_t for a partition
 * @part: partition to allocate dev_t for
 * @devt: out parameter for resulting dev_t
 *
 * Allocate a dev_t for block device.
 *
 * RETURNS:
 * 0 on success, allocated dev_t is returned in *@devt.  -errno on
 * failure.
 *
 * CONTEXT:
 * Might sleep.
 */
int blk_alloc_devt(struct hd_struct *part, dev_t *devt)
{
	struct gendisk *disk = part_to_disk(part);
	int idx, rc;

	/* in consecutive minor range? */
	if (part->partno < disk->minors) {
		*devt = MKDEV(disk->major, disk->first_minor + part->partno);
		return 0;
	}

	/* allocate ext devt */
	do {
		if (!idr_pre_get(&ext_devt_idr, GFP_KERNEL))
			return -ENOMEM;
		rc = idr_get_new(&ext_devt_idr, part, &idx);
	} while (rc == -EAGAIN);

	if (rc)
		return rc;

	if (idx > MAX_EXT_DEVT) {
		idr_remove(&ext_devt_idr, idx);
		return -EBUSY;
	}

	*devt = MKDEV(BLOCK_EXT_MAJOR, blk_mangle_minor(idx));
	return 0;
}

/**
 * blk_free_devt - free a dev_t
 * @devt: dev_t to free
 *
 * Free @devt which was allocated using blk_alloc_devt().
 *
 * CONTEXT:
 * Might sleep.
 */
void blk_free_devt(dev_t devt)
{
	might_sleep();

	if (devt == MKDEV(0, 0))
		return;

	if (MAJOR(devt) == BLOCK_EXT_MAJOR) {
		mutex_lock(&ext_devt_mutex);
		idr_remove(&ext_devt_idr, blk_mangle_minor(MINOR(devt)));
		mutex_unlock(&ext_devt_mutex);
	}
}

static char *bdevt_str(dev_t devt, char *buf)
{
	if (MAJOR(devt) <= 0xff && MINOR(devt) <= 0xff) {
		char tbuf[BDEVT_SIZE];
		snprintf(tbuf, BDEVT_SIZE, "%02x%02x", MAJOR(devt), MINOR(devt));
		snprintf(buf, BDEVT_SIZE, "%-9s", tbuf);
	} else
		snprintf(buf, BDEVT_SIZE, "%03x:%05x", MAJOR(devt), MINOR(devt));

	return buf;
}

/*
 * Register device numbers dev..(dev+range-1)
 * range must be nonzero
 * The hash chain is sorted on range, so that subranges can override.
 */
void blk_register_region(dev_t devt, unsigned long range, struct module *module,
			 struct kobject *(*probe)(dev_t, int *, void *),
			 int (*lock)(dev_t, void *), void *data)
{
	kobj_map(bdev_map, devt, range, module, probe, lock, data);
}

EXPORT_SYMBOL(blk_register_region);

void blk_unregister_region(dev_t devt, unsigned long range)
{
	kobj_unmap(bdev_map, devt, range);
}

EXPORT_SYMBOL(blk_unregister_region);

static struct kobject *exact_match(dev_t devt, int *partno, void *data)
{
	struct gendisk *p = data;

	return &disk_to_dev(p)->kobj;
}

static int exact_lock(dev_t devt, void *data)
{
	struct gendisk *p = data;

	if (!get_disk(p))
		return -1;
	return 0;
}

void register_disk(struct gendisk *disk)
{
	struct device *ddev = disk_to_dev(disk);
	struct block_device *bdev;
	struct disk_part_iter piter;
	struct hd_struct *part;
	int err;

	ddev->parent = disk->driverfs_dev;

	dev_set_name(ddev, disk->disk_name);

	/* delay uevents, until we scanned partition table */
	dev_set_uevent_suppress(ddev, 1);

	if (device_add(ddev))
		return;
	if (!sysfs_deprecated) {
		err = sysfs_create_link(block_depr, &ddev->kobj,
					kobject_name(&ddev->kobj));
		if (err) {
			device_del(ddev);
			return;
		}
	}

#ifdef CONFIG_FS_SEL_READAHEAD         
       create_readahead_proc(dev_name(ddev));  
#endif

	disk->part0.holder_dir = kobject_create_and_add("holders", &ddev->kobj);
	disk->slave_dir = kobject_create_and_add("slaves", &ddev->kobj);

	/* No minors to use for partitions */
	if (!disk_partitionable(disk))
		goto exit;

	/* No such device (e.g., media were just removed) */
	if (!get_capacity(disk))
		goto exit;

	bdev = bdget_disk(disk, 0);
	if (!bdev)
		goto exit;

	bdev->bd_invalidated = 1;
	err = blkdev_get(bdev, FMODE_READ, NULL);
	if (err < 0)
		goto exit;
	blkdev_put(bdev, FMODE_READ);

exit:
	/* announce disk after possible partitions are created */
	dev_set_uevent_suppress(ddev, 0);
	kobject_uevent(&ddev->kobj, KOBJ_ADD);

	/* announce possible partitions */
	disk_part_iter_init(&piter, disk, 0);
	while ((part = disk_part_iter_next(&piter)))
		kobject_uevent(&part_to_dev(part)->kobj, KOBJ_ADD);
	disk_part_iter_exit(&piter);
}

/**
 * add_disk - add partitioning information to kernel list
 * @disk: per-device partitioning information
 *
 * This function registers the partitioning information in @disk
 * with the kernel.
 *
 * FIXME: error handling
 */
void add_disk(struct gendisk *disk)
{
	struct backing_dev_info *bdi;
	dev_t devt;
	int retval;

	/* minors == 0 indicates to use ext devt from part0 and should
	 * be accompanied with EXT_DEVT flag.  Make sure all
	 * parameters make sense.
	 */
	WARN_ON(disk->minors && !(disk->major || disk->first_minor));
	WARN_ON(!disk->minors && !(disk->flags & GENHD_FL_EXT_DEVT));

	disk->flags |= GENHD_FL_UP;

	retval = blk_alloc_devt(&disk->part0, &devt);
	if (retval) {
		WARN_ON(1);
		return;
	}
	disk_to_dev(disk)->devt = devt;

	/* ->major and ->first_minor aren't supposed to be
	 * dereferenced from here on, but set them just in case.
	 */
	disk->major = MAJOR(devt);
	disk->first_minor = MINOR(devt);

	disk_alloc_events(disk);

	/* Register BDI before referencing it from bdev */ 
	bdi = &disk->queue->backing_dev_info;
	bdi_register_dev(bdi, disk_devt(disk));

	blk_register_region(disk_devt(disk), disk->minors, NULL,
			    exact_match, exact_lock, disk);
	register_disk(disk);
	blk_register_queue(disk);

	/*
	 * Take an extra ref on queue which will be put on disk_release()
	 * so that it sticks around as long as @disk is there.
	 */
	WARN_ON_ONCE(blk_get_queue(disk->queue));

	retval = sysfs_create_link(&disk_to_dev(disk)->kobj, &bdi->dev->kobj,
				   "bdi");
	WARN_ON(retval);

	disk_add_events(disk);
}
EXPORT_SYMBOL(add_disk);

void del_gendisk(struct gendisk *disk)
{
	struct disk_part_iter piter;
	struct hd_struct *part;

	disk_del_events(disk);

	/* invalidate stuff */
	disk_part_iter_init(&piter, disk,
			     DISK_PITER_INCL_EMPTY | DISK_PITER_REVERSE);
	while ((part = disk_part_iter_next(&piter))) {
		invalidate_partition(disk, part->partno);
		delete_partition(disk, part->partno);
	}
	disk_part_iter_exit(&piter);

#ifdef CONFIG_FS_SEL_READAHEAD
	remove_readahead_proc(dev_name(disk_to_dev(disk)));
#endif

	invalidate_partition(disk, 0);
	blk_free_devt(disk_to_dev(disk)->devt);
	set_capacity(disk, 0);
	disk->flags &= ~GENHD_FL_UP;

	sysfs_remove_link(&disk_to_dev(disk)->kobj, "bdi");
	bdi_unregister(&disk->queue->backing_dev_info);
	blk_unregister_queue(disk);
	blk_unregister_region(disk_devt(disk), disk->minors);

	part_stat_set_all(&disk->part0, 0);
	disk->part0.stamp = 0;

	kobject_put(disk->part0.holder_dir);
	kobject_put(disk->slave_dir);
	disk->driverfs_dev = NULL;
	if (!sysfs_deprecated)
		sysfs_remove_link(block_depr, dev_name(disk_to_dev(disk)));
	device_del(disk_to_dev(disk));
}
EXPORT_SYMBOL(del_gendisk);

#ifdef CONFIG_FS_SEL_READAHEAD
atomic_t *
disk_name_from_dev(dev_t dev)
{
	struct block_device *bdev = bdget(dev);
	char buff[BDEVNAME_SIZE]={0};
	atomic_t *pstate = NULL;
	if (bdev) {
		if (MAJOR(dev)){
			bdevname(bdev, buff);
			pstate = get_readahead_entry(buff);
		}
		bdput(bdev);
	}
	return pstate;
}

EXPORT_SYMBOL(disk_name_from_dev);
#endif

/**
 * get_gendisk - get partitioning information for a given device
 * @devt: device to get partitioning information for
 * @partno: returned partition index
 *
 * This function gets the structure containing partitioning
 * information for the given device @devt.
 */
struct gendisk *get_gendisk(dev_t devt, int *partno)
{
	struct gendisk *disk = NULL;

	if (MAJOR(devt) != BLOCK_EXT_MAJOR) {
		struct kobject *kobj;

		kobj = kobj_lookup(bdev_map, devt, partno);
		if (kobj)
			disk = dev_to_disk(kobj_to_dev(kobj));
	} else {
		struct hd_struct *part;

		mutex_lock(&ext_devt_mutex);
		part = idr_find(&ext_devt_idr, blk_mangle_minor(MINOR(devt)));
		if (part && get_disk(part_to_disk(part))) {
			*partno = part->partno;
			disk = part_to_disk(part);
		}
		mutex_unlock(&ext_devt_mutex);
	}

	return disk;
}
EXPORT_SYMBOL(get_gendisk);

/**
 * bdget_disk - do bdget() by gendisk and partition number
 * @disk: gendisk of interest
 * @partno: partition number
 *
 * Find partition @partno from @disk, do bdget() on it.
 *
 * CONTEXT:
 * Don't care.
 *
 * RETURNS:
 * Resulting block_device on success, NULL on failure.
 */
struct block_device *bdget_disk(struct gendisk *disk, int partno)
{
	struct hd_struct *part;
	struct block_device *bdev = NULL;

	part = disk_get_part(disk, partno);
	if (part)
		bdev = bdget(part_devt(part));
	disk_put_part(part);

	return bdev;
}
EXPORT_SYMBOL(bdget_disk);

/*
 * print a full list of all partitions - intended for places where the root
 * filesystem can't be mounted and thus to give the victim some idea of what
 * went wrong
 */
void __init printk_all_partitions(void)
{
	struct class_dev_iter iter;
	struct device *dev;

	class_dev_iter_init(&iter, &block_class, NULL, &disk_type);
	while ((dev = class_dev_iter_next(&iter))) {
		struct gendisk *disk = dev_to_disk(dev);
		struct disk_part_iter piter;
		struct hd_struct *part;
		char name_buf[BDEVNAME_SIZE];
		char devt_buf[BDEVT_SIZE];
		char uuid_buf[PARTITION_META_INFO_UUIDLTH * 2 + 5];

		/*
		 * Don't show empty devices or things that have been
		 * suppressed
		 */
		if (get_capacity(disk) == 0 ||
		    (disk->flags & GENHD_FL_SUPPRESS_PARTITION_INFO))
			continue;

		/*
		 * Note, unlike /proc/partitions, I am showing the
		 * numbers in hex - the same format as the root=
		 * option takes.
		 */
		disk_part_iter_init(&piter, disk, DISK_PITER_INCL_PART0);
		while ((part = disk_part_iter_next(&piter))) {
			bool is_part0 = part == &disk->part0;

			uuid_buf[0] = '\0';
			if (part->info)
				snprintf(uuid_buf, sizeof(uuid_buf), "%pU",
					 part->info->uuid);

			printk("%s%s %10llu %s %s", is_part0 ? "" : "  ",
			       bdevt_str(part_devt(part), devt_buf),
			       (unsigned long long)part->nr_sects >> 1,
			       disk_name(disk, part->partno, name_buf),
			       uuid_buf);
			if (is_part0) {
				if (disk->driverfs_dev != NULL &&
				    disk->driverfs_dev->driver != NULL)
					printk(" driver: %s\n",
					      disk->driverfs_dev->driver->name);
				else
					printk(" (driver?)\n");
			} else
				printk("\n");
		}
		disk_part_iter_exit(&piter);
	}
	class_dev_iter_exit(&iter);
}

#ifdef CONFIG_PROC_FS
/* iterator */
static void *disk_seqf_start(struct seq_file *seqf, loff_t *pos)
{
	loff_t skip = *pos;
	struct class_dev_iter *iter;
	struct device *dev;

	iter = kmalloc(sizeof(*iter), GFP_KERNEL);
	if (!iter)
		return ERR_PTR(-ENOMEM);

	seqf->private = iter;
	class_dev_iter_init(iter, &block_class, NULL, &disk_type);
	do {
		dev = class_dev_iter_next(iter);
		if (!dev)
			return NULL;
	} while (skip--);

	return dev_to_disk(dev);
}

static void *disk_seqf_next(struct seq_file *seqf, void *v, loff_t *pos)
{
	struct device *dev;

	(*pos)++;
	dev = class_dev_iter_next(seqf->private);
	if (dev)
		return dev_to_disk(dev);

	return NULL;
}

static void disk_seqf_stop(struct seq_file *seqf, void *v)
{
	struct class_dev_iter *iter = seqf->private;

	/* stop is called even after start failed :-( */
	if (iter) {
		class_dev_iter_exit(iter);
		kfree(iter);
	}
}

static void *show_partition_start(struct seq_file *seqf, loff_t *pos)
{
	static void *p;

	p = disk_seqf_start(seqf, pos);
	if (!IS_ERR_OR_NULL(p) && !*pos)
		seq_puts(seqf, "major minor  #blocks  name\n\n");
	return p;
}

static int show_partition(struct seq_file *seqf, void *v)
{
	struct gendisk *sgp = v;
	struct disk_part_iter piter;
	struct hd_struct *part;
	char buf[BDEVNAME_SIZE];

	/* Don't show non-partitionable removeable devices or empty devices */
	if (!get_capacity(sgp) || (!disk_partitionable(sgp) &&
				   (sgp->flags & GENHD_FL_REMOVABLE)))
		return 0;
	if (sgp->flags & GENHD_FL_SUPPRESS_PARTITION_INFO)
		return 0;

	/* show the full disk and all non-0 size partitions of it */
	disk_part_iter_init(&piter, sgp, DISK_PITER_INCL_PART0);
	while ((part = disk_part_iter_next(&piter)))
		seq_printf(seqf, "%4d  %7d %10llu %s\n",
			   MAJOR(part_devt(part)), MINOR(part_devt(part)),
			   (unsigned long long)part->nr_sects >> 1,
			   disk_name(sgp, part->partno, buf));
	disk_part_iter_exit(&piter);

	return 0;
}

static const struct seq_operations partitions_op = {
	.start	= show_partition_start,
	.next	= disk_seqf_next,
	.stop	= disk_seqf_stop,
	.show	= show_partition
};

static int partitions_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &partitions_op);
}

static const struct file_operations proc_partitions_operations = {
	.open		= partitions_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};
#endif


static struct kobject *base_probe(dev_t devt, int *partno, void *data)
{
	if (request_module("block-major-%d-%d", MAJOR(devt), MINOR(devt)) > 0)
		/* Make old-style 2.4 aliases work */
		request_module("block-major-%d", MAJOR(devt));
	return NULL;
}

static int __init genhd_device_init(void)
{
	int error;

	block_class.dev_kobj = sysfs_dev_block_kobj;
	error = class_register(&block_class);
	if (unlikely(error))
		return error;
	bdev_map = kobj_map_init(base_probe, &block_class_lock);
	blk_dev_init();

	register_blkdev(BLOCK_EXT_MAJOR, "blkext");

	/* create top-level block dir */
	if (!sysfs_deprecated)
		block_depr = kobject_create_and_add("block", NULL);

#ifdef CONFIG_BOOTPROFILE
	sec_bp_start();
#endif /* CONFIG_BOOTPROFILE */

	return 0;
}

subsys_initcall(genhd_device_init);

static ssize_t disk_range_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", disk->minors);
}

static ssize_t disk_ext_range_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", disk_max_parts(disk));
}

static ssize_t disk_removable_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n",
		       (disk->flags & GENHD_FL_REMOVABLE ? 1 : 0));
}

static ssize_t disk_ro_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", get_disk_ro(disk) ? 1 : 0);
}

static ssize_t disk_capability_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%x\n", disk->flags);
}

static ssize_t disk_alignment_offset_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", queue_alignment_offset(disk->queue));
}

static ssize_t disk_discard_alignment_show(struct device *dev,
					   struct device_attribute *attr,
					   char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", queue_discard_alignment(disk->queue));
}

static DEVICE_ATTR(range, S_IRUGO, disk_range_show, NULL);
static DEVICE_ATTR(ext_range, S_IRUGO, disk_ext_range_show, NULL);
static DEVICE_ATTR(removable, S_IRUGO, disk_removable_show, NULL);
static DEVICE_ATTR(ro, S_IRUGO, disk_ro_show, NULL);
static DEVICE_ATTR(size, S_IRUGO, part_size_show, NULL);
static DEVICE_ATTR(alignment_offset, S_IRUGO, disk_alignment_offset_show, NULL);
static DEVICE_ATTR(discard_alignment, S_IRUGO, disk_discard_alignment_show,
		   NULL);
static DEVICE_ATTR(capability, S_IRUGO, disk_capability_show, NULL);
static DEVICE_ATTR(stat, S_IRUGO, part_stat_show, NULL);
static DEVICE_ATTR(inflight, S_IRUGO, part_inflight_show, NULL);
#ifdef CONFIG_FAIL_MAKE_REQUEST
static struct device_attribute dev_attr_fail =
	__ATTR(make-it-fail, S_IRUGO|S_IWUSR, part_fail_show, part_fail_store);
#endif
#ifdef CONFIG_FAIL_IO_TIMEOUT
static struct device_attribute dev_attr_fail_timeout =
	__ATTR(io-timeout-fail,  S_IRUGO|S_IWUSR, part_timeout_show,
		part_timeout_store);
#endif

static struct attribute *disk_attrs[] = {
	&dev_attr_range.attr,
	&dev_attr_ext_range.attr,
	&dev_attr_removable.attr,
	&dev_attr_ro.attr,
	&dev_attr_size.attr,
	&dev_attr_alignment_offset.attr,
	&dev_attr_discard_alignment.attr,
	&dev_attr_capability.attr,
	&dev_attr_stat.attr,
	&dev_attr_inflight.attr,
#ifdef CONFIG_FAIL_MAKE_REQUEST
	&dev_attr_fail.attr,
#endif
#ifdef CONFIG_FAIL_IO_TIMEOUT
	&dev_attr_fail_timeout.attr,
#endif
	NULL
};

static struct attribute_group disk_attr_group = {
	.attrs = disk_attrs,
};

static const struct attribute_group *disk_attr_groups[] = {
	&disk_attr_group,
	NULL
};

static void disk_free_ptbl_rcu_cb(struct rcu_head *head)
{
	struct disk_part_tbl *ptbl =
		container_of(head, struct disk_part_tbl, rcu_head);

	kfree(ptbl);
}

/**
 * disk_replace_part_tbl - replace disk->part_tbl in RCU-safe way
 * @disk: disk to replace part_tbl for
 * @new_ptbl: new part_tbl to install
 *
 * Replace disk->part_tbl with @new_ptbl in RCU-safe way.  The
 * original ptbl is freed using RCU callback.
 *
 * LOCKING:
 * Matching bd_mutx locked.
 */
static void disk_replace_part_tbl(struct gendisk *disk,
				  struct disk_part_tbl *new_ptbl)
{
	struct disk_part_tbl *old_ptbl = disk->part_tbl;

	rcu_assign_pointer(disk->part_tbl, new_ptbl);

	if (old_ptbl) {
		rcu_assign_pointer(old_ptbl->last_lookup, NULL);
		call_rcu(&old_ptbl->rcu_head, disk_free_ptbl_rcu_cb);
	}
}

/**
 * disk_expand_part_tbl - expand disk->part_tbl
 * @disk: disk to expand part_tbl for
 * @partno: expand such that this partno can fit in
 *
 * Expand disk->part_tbl such that @partno can fit in.  disk->part_tbl
 * uses RCU to allow unlocked dereferencing for stats and other stuff.
 *
 * LOCKING:
 * Matching bd_mutex locked, might sleep.
 *
 * RETURNS:
 * 0 on success, -errno on failure.
 */
int disk_expand_part_tbl(struct gendisk *disk, int partno)
{
	struct disk_part_tbl *old_ptbl = disk->part_tbl;
	struct disk_part_tbl *new_ptbl;
	int len = old_ptbl ? old_ptbl->len : 0;
	int target = partno + 1;
	size_t size;
	int i;

	/* disk_max_parts() is zero during initialization, ignore if so */
	if (disk_max_parts(disk) && target > disk_max_parts(disk))
		return -EINVAL;

	if (target <= len)
		return 0;

	size = sizeof(*new_ptbl) + target * sizeof(new_ptbl->part[0]);
	new_ptbl = kzalloc_node(size, GFP_KERNEL, disk->node_id);
	if (!new_ptbl)
		return -ENOMEM;

	new_ptbl->len = target;

	for (i = 0; i < len; i++)
		rcu_assign_pointer(new_ptbl->part[i], old_ptbl->part[i]);

	disk_replace_part_tbl(disk, new_ptbl);
	return 0;
}

static void disk_release(struct device *dev)
{
	struct gendisk *disk = dev_to_disk(dev);

	disk_release_events(disk);
	kfree(disk->random);
	disk_replace_part_tbl(disk, NULL);
	free_part_stats(&disk->part0);
	free_part_info(&disk->part0);
	if (disk->queue)
		blk_put_queue(disk->queue);
	kfree(disk);
}
struct class block_class = {
	.name		= "block",
};

static char *block_devnode(struct device *dev, mode_t *mode)
{
	struct gendisk *disk = dev_to_disk(dev);

	if (disk->devnode)
		return disk->devnode(disk, mode);
	return NULL;
}

static struct device_type disk_type = {
	.name		= "disk",
	.groups		= disk_attr_groups,
	.release	= disk_release,
	.devnode	= block_devnode,
};

#ifdef CONFIG_PROC_FS
/*
 * aggregate disk stat collector.  Uses the same stats that the sysfs
 * entries do, above, but makes them available through one seq_file.
 *
 * The output looks suspiciously like /proc/partitions with a bunch of
 * extra fields.
 */
static int diskstats_show(struct seq_file *seqf, void *v)
{
	struct gendisk *gp = v;
	struct disk_part_iter piter;
	struct hd_struct *hd;
	char buf[BDEVNAME_SIZE];
	int cpu;

	/*
	if (&disk_to_dev(gp)->kobj.entry == block_class.devices.next)
		seq_puts(seqf,	"major minor name"
				"     rio rmerge rsect ruse wio wmerge "
				"wsect wuse running use aveq"
				"\n\n");
	*/
 
	disk_part_iter_init(&piter, gp, DISK_PITER_INCL_EMPTY_PART0);
	while ((hd = disk_part_iter_next(&piter))) {
		cpu = part_stat_lock();
		part_round_stats(cpu, hd);
		part_stat_unlock();
		seq_printf(seqf, "%4d %7d %s %lu %lu %llu "
			   "%u %lu %lu %llu %u %u %u %u\n",
			   MAJOR(part_devt(hd)), MINOR(part_devt(hd)),
			   disk_name(gp, hd->partno, buf),
			   part_stat_read(hd, ios[READ]),
			   part_stat_read(hd, merges[READ]),
			   (unsigned long long)part_stat_read(hd, sectors[READ]),
			   jiffies_to_msecs(part_stat_read(hd, ticks[READ])),
			   part_stat_read(hd, ios[WRITE]),
			   part_stat_read(hd, merges[WRITE]),
			   (unsigned long long)part_stat_read(hd, sectors[WRITE]),
			   jiffies_to_msecs(part_stat_read(hd, ticks[WRITE])),
			   part_in_flight(hd),
			   jiffies_to_msecs(part_stat_read(hd, io_ticks)),
			   jiffies_to_msecs(part_stat_read(hd, time_in_queue))
			);
	}
	disk_part_iter_exit(&piter);
 
	return 0;
}

static const struct seq_operations diskstats_op = {
	.start	= disk_seqf_start,
	.next	= disk_seqf_next,
	.stop	= disk_seqf_stop,
	.show	= diskstats_show
};

static int diskstats_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &diskstats_op);
}

static const struct file_operations proc_diskstats_operations = {
	.open		= diskstats_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_genhd_init(void)
{
	proc_create("diskstats", 0, NULL, &proc_diskstats_operations);
	proc_create("partitions", 0, NULL, &proc_partitions_operations);
	return 0;
}
module_init(proc_genhd_init);
#endif /* CONFIG_PROC_FS */

dev_t blk_lookup_devt(const char *name, int partno)
{
	dev_t devt = MKDEV(0, 0);
	struct class_dev_iter iter;
	struct device *dev;

	class_dev_iter_init(&iter, &block_class, NULL, &disk_type);
	while ((dev = class_dev_iter_next(&iter))) {
		struct gendisk *disk = dev_to_disk(dev);
		struct hd_struct *part;

		if (strcmp(dev_name(dev), name))
			continue;

		if (partno < disk->minors) {
			/* We need to return the right devno, even
			 * if the partition doesn't exist yet.
			 */
			devt = MKDEV(MAJOR(dev->devt),
				     MINOR(dev->devt) + partno);
			break;
		}
		part = disk_get_part(disk, partno);
		if (part) {
			devt = part_devt(part);
			disk_put_part(part);
			break;
		}
		disk_put_part(part);
	}
	class_dev_iter_exit(&iter);
	return devt;
}
EXPORT_SYMBOL(blk_lookup_devt);

struct gendisk *alloc_disk(int minors)
{
	return alloc_disk_node(minors, -1);
}
EXPORT_SYMBOL(alloc_disk);

struct gendisk *alloc_disk_node(int minors, int node_id)
{
	struct gendisk *disk;

	disk = kmalloc_node(sizeof(struct gendisk),
				GFP_KERNEL | __GFP_ZERO, node_id);
	if (disk) {
		if (!init_part_stats(&disk->part0)) {
			kfree(disk);
			return NULL;
		}
		disk->node_id = node_id;
		if (disk_expand_part_tbl(disk, 0)) {
			free_part_stats(&disk->part0);
			kfree(disk);
			return NULL;
		}
		disk->part_tbl->part[0] = &disk->part0;

		hd_ref_init(&disk->part0);

		disk->minors = minors;
		rand_initialize_disk(disk);
		disk_to_dev(disk)->class = &block_class;
		disk_to_dev(disk)->type = &disk_type;
		device_initialize(disk_to_dev(disk));
	}
	return disk;
}
EXPORT_SYMBOL(alloc_disk_node);

struct kobject *get_disk(struct gendisk *disk)
{
	struct module *owner;
	struct kobject *kobj;

	if (!disk->fops)
		return NULL;
	owner = disk->fops->owner;
	if (owner && !try_module_get(owner))
		return NULL;
	kobj = kobject_get(&disk_to_dev(disk)->kobj);
	if (kobj == NULL) {
		module_put(owner);
		return NULL;
	}
	return kobj;

}

EXPORT_SYMBOL(get_disk);

void put_disk(struct gendisk *disk)
{
	if (disk)
		kobject_put(&disk_to_dev(disk)->kobj);
}

EXPORT_SYMBOL(put_disk);

static void set_disk_ro_uevent(struct gendisk *gd, int ro)
{
	char event[] = "DISK_RO=1";
	char *envp[] = { event, NULL };

	if (!ro)
		event[8] = '0';
	kobject_uevent_env(&disk_to_dev(gd)->kobj, KOBJ_CHANGE, envp);
}

void set_device_ro(struct block_device *bdev, int flag)
{
	bdev->bd_part->policy = flag;
}

EXPORT_SYMBOL(set_device_ro);

void set_disk_ro(struct gendisk *disk, int flag)
{
	struct disk_part_iter piter;
	struct hd_struct *part;

	if (disk->part0.policy != flag) {
		set_disk_ro_uevent(disk, flag);
		disk->part0.policy = flag;
	}

	disk_part_iter_init(&piter, disk, DISK_PITER_INCL_EMPTY);
	while ((part = disk_part_iter_next(&piter)))
		part->policy = flag;
	disk_part_iter_exit(&piter);
}

EXPORT_SYMBOL(set_disk_ro);

int bdev_read_only(struct block_device *bdev)
{
	if (!bdev)
		return 0;
	return bdev->bd_part->policy;
}

EXPORT_SYMBOL(bdev_read_only);

int invalidate_partition(struct gendisk *disk, int partno)
{
	int res = 0;
	struct block_device *bdev = bdget_disk(disk, partno);
	if (bdev) {
		fsync_bdev(bdev);
		res = __invalidate_device(bdev, true);
		bdput(bdev);
	}
	return res;
}

EXPORT_SYMBOL(invalidate_partition);

/*
 * Disk events - monitor disk events like media change and eject request.
 */
struct disk_events {
	struct list_head	node;		/* all disk_event's */
	struct gendisk		*disk;		/* the associated disk */
	spinlock_t		lock;

	struct mutex		block_mutex;	/* protects blocking */
	int			block;		/* event blocking depth */
	unsigned int		pending;	/* events already sent out */
	unsigned int		clearing;	/* events being cleared */

	long			poll_msecs;	/* interval, -1 for default */
	struct delayed_work	dwork;
};

static const char *disk_events_strs[] = {
	[ilog2(DISK_EVENT_MEDIA_CHANGE)]	= "media_change",
	[ilog2(DISK_EVENT_EJECT_REQUEST)]	= "eject_request",
};

static char *disk_uevents[] = {
	[ilog2(DISK_EVENT_MEDIA_CHANGE)]	= "DISK_MEDIA_CHANGE=1",
	[ilog2(DISK_EVENT_EJECT_REQUEST)]	= "DISK_EJECT_REQUEST=1",
};

/* list of all disk_events */
static DEFINE_MUTEX(disk_events_mutex);
static LIST_HEAD(disk_events);

/* disable in-kernel polling by default */
static unsigned long disk_events_dfl_poll_msecs	= 0;

static unsigned long disk_events_poll_jiffies(struct gendisk *disk)
{
	struct disk_events *ev = disk->ev;
	long intv_msecs = 0;

	/*
	 * If device-specific poll interval is set, always use it.  If
	 * the default is being used, poll iff there are events which
	 * can't be monitored asynchronously.
	 */
	if (ev->poll_msecs >= 0)
		intv_msecs = ev->poll_msecs;
	else if (disk->events & ~disk->async_events)
		intv_msecs = disk_events_dfl_poll_msecs;

	return msecs_to_jiffies(intv_msecs);
}

/**
 * disk_block_events - block and flush disk event checking
 * @disk: disk to block events for
 *
 * On return from this function, it is guaranteed that event checking
 * isn't in progress and won't happen until unblocked by
 * disk_unblock_events().  Events blocking is counted and the actual
 * unblocking happens after the matching number of unblocks are done.
 *
 * Note that this intentionally does not block event checking from
 * disk_clear_events().
 *
 * CONTEXT:
 * Might sleep.
 */
void disk_block_events(struct gendisk *disk)
{
	struct disk_events *ev = disk->ev;
	unsigned long flags;
	bool cancel;

	if (!ev)
		return;

	/*
	 * Outer mutex ensures that the first blocker completes canceling
	 * the event work before further blockers are allowed to finish.
	 */
	mutex_lock(&ev->block_mutex);

	spin_lock_irqsave(&ev->lock, flags);
	cancel = !ev->block++;
	spin_unlock_irqrestore(&ev->lock, flags);

	if (cancel)
		cancel_delayed_work_sync(&disk->ev->dwork);

	mutex_unlock(&ev->block_mutex);
}

static void __disk_unblock_events(struct gendisk *disk, bool check_now)
{
	struct disk_events *ev = disk->ev;
	unsigned long intv;
	unsigned long flags;

	spin_lock_irqsave(&ev->lock, flags);

	if (WARN_ON_ONCE(ev->block <= 0))
		goto out_unlock;

	if (--ev->block)
		goto out_unlock;

	/*
	 * Not exactly a latency critical operation, set poll timer
	 * slack to 25% and kick event check.
	 */
	intv = disk_events_poll_jiffies(disk);
	set_timer_slack(&ev->dwork.timer, intv / 4);
	if (check_now)
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, 0);
	else if (intv)
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, intv);
out_unlock:
	spin_unlock_irqrestore(&ev->lock, flags);
}

/**
 * disk_unblock_events - unblock disk event checking
 * @disk: disk to unblock events for
 *
 * Undo disk_block_events().  When the block count reaches zero, it
 * starts events polling if configured.
 *
 * CONTEXT:
 * Don't care.  Safe to call from irq context.
 */
void disk_unblock_events(struct gendisk *disk)
{
	if (disk->ev)
		__disk_unblock_events(disk, false);
}

/**
 * disk_check_events - schedule immediate event checking
 * @disk: disk to check events for
 *
 * Schedule immediate event checking on @disk if not blocked.
 *
 * CONTEXT:
 * Don't care.  Safe to call from irq context.
 */
void disk_check_events(struct gendisk *disk)
{
	struct disk_events *ev = disk->ev;
	unsigned long flags;

	if (!ev)
		return;

	spin_lock_irqsave(&ev->lock, flags);
	if (!ev->block) {
		cancel_delayed_work(&ev->dwork);
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, 0);
	}
	spin_unlock_irqrestore(&ev->lock, flags);
}
EXPORT_SYMBOL_GPL(disk_check_events);

/**
 * disk_clear_events - synchronously check, clear and return pending events
 * @disk: disk to fetch and clear events from
 * @mask: mask of events to be fetched and clearted
 *
 * Disk events are synchronously checked and pending events in @mask
 * are cleared and returned.  This ignores the block count.
 *
 * CONTEXT:
 * Might sleep.
 */
unsigned int disk_clear_events(struct gendisk *disk, unsigned int mask)
{
	const struct block_device_operations *bdops = disk->fops;
	struct disk_events *ev = disk->ev;
	unsigned int pending;

	if (!ev) {
		/* for drivers still using the old ->media_changed method */
		if ((mask & DISK_EVENT_MEDIA_CHANGE) &&
		    bdops->media_changed && bdops->media_changed(disk))
			return DISK_EVENT_MEDIA_CHANGE;
		return 0;
	}

	/* tell the workfn about the events being cleared */
	spin_lock_irq(&ev->lock);
	ev->clearing |= mask;
	spin_unlock_irq(&ev->lock);

	/* uncondtionally schedule event check and wait for it to finish */
	disk_block_events(disk);
	queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, 0);
	flush_delayed_work(&ev->dwork);
	__disk_unblock_events(disk, false);

	/* then, fetch and clear pending events */
	spin_lock_irq(&ev->lock);
	WARN_ON_ONCE(ev->clearing & mask);	/* cleared by workfn */
	pending = ev->pending & mask;
	ev->pending &= ~mask;
	spin_unlock_irq(&ev->lock);

	return pending;
}

static void disk_events_workfn(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct disk_events *ev = container_of(dwork, struct disk_events, dwork);
	struct gendisk *disk = ev->disk;
	char *envp[ARRAY_SIZE(disk_uevents) + 1] = { };
	unsigned int clearing = ev->clearing;
	unsigned int events;
	unsigned long intv;
	int nr_events = 0, i;

	/* check events */
	events = disk->fops->check_events(disk, clearing);

	/* accumulate pending events and schedule next poll if necessary */
	spin_lock_irq(&ev->lock);

	events &= ~ev->pending;
	ev->pending |= events;
	ev->clearing &= ~clearing;

	intv = disk_events_poll_jiffies(disk);
	if (!ev->block && intv)
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, intv);

	spin_unlock_irq(&ev->lock);

	/*
	 * Tell userland about new events.  Only the events listed in
	 * @disk->events are reported.  Unlisted events are processed the
	 * same internally but never get reported to userland.
	 */
	for (i = 0; i < ARRAY_SIZE(disk_uevents); i++)
		if (events & disk->events & (1 << i))
			envp[nr_events++] = disk_uevents[i];

	if (nr_events)
		kobject_uevent_env(&disk_to_dev(disk)->kobj, KOBJ_CHANGE, envp);
}

/*
 * A disk events enabled device has the following sysfs nodes under
 * its /sys/block/X/ directory.
 *
 * events		: list of all supported events
 * events_async		: list of events which can be detected w/o polling
 * events_poll_msecs	: polling interval, 0: disable, -1: system default
 */
static ssize_t __disk_events_show(unsigned int events, char *buf)
{
	const char *delim = "";
	ssize_t pos = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(disk_events_strs); i++)
		if (events & (1 << i)) {
			pos += sprintf(buf + pos, "%s%s",
				       delim, disk_events_strs[i]);
			delim = " ";
		}
	if (pos)
		pos += sprintf(buf + pos, "\n");
	return pos;
}

static ssize_t disk_events_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return __disk_events_show(disk->events, buf);
}

static ssize_t disk_events_async_show(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return __disk_events_show(disk->async_events, buf);
}

static ssize_t disk_events_poll_msecs_show(struct device *dev,
					   struct device_attribute *attr,
					   char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%ld\n", disk->ev->poll_msecs);
}

static ssize_t disk_events_poll_msecs_store(struct device *dev,
					    struct device_attribute *attr,
					    const char *buf, size_t count)
{
	struct gendisk *disk = dev_to_disk(dev);
	long intv;

	if (!count || !sscanf(buf, "%ld", &intv))
		return -EINVAL;

	if (intv < 0 && intv != -1)
		return -EINVAL;

	disk_block_events(disk);
	disk->ev->poll_msecs = intv;
	__disk_unblock_events(disk, true);

	return count;
}

static const DEVICE_ATTR(events, S_IRUGO, disk_events_show, NULL);
static const DEVICE_ATTR(events_async, S_IRUGO, disk_events_async_show, NULL);
static const DEVICE_ATTR(events_poll_msecs, S_IRUGO|S_IWUSR,
			 disk_events_poll_msecs_show,
			 disk_events_poll_msecs_store);

static const struct attribute *disk_events_attrs[] = {
	&dev_attr_events.attr,
	&dev_attr_events_async.attr,
	&dev_attr_events_poll_msecs.attr,
	NULL,
};

/*
 * The default polling interval can be specified by the kernel
 * parameter block.events_dfl_poll_msecs which defaults to 0
 * (disable).  This can also be modified runtime by writing to
 * /sys/module/block/events_dfl_poll_msecs.
 */
static int disk_events_set_dfl_poll_msecs(const char *val,
					  const struct kernel_param *kp)
{
	struct disk_events *ev;
	int ret;

	ret = param_set_ulong(val, kp);
	if (ret < 0)
		return ret;

	mutex_lock(&disk_events_mutex);

	list_for_each_entry(ev, &disk_events, node)
		disk_check_events(ev->disk);

	mutex_unlock(&disk_events_mutex);

	return 0;
}

static const struct kernel_param_ops disk_events_dfl_poll_msecs_param_ops = {
	.set	= disk_events_set_dfl_poll_msecs,
	.get	= param_get_ulong,
};

#undef MODULE_PARAM_PREFIX
#define MODULE_PARAM_PREFIX	"block."

module_param_cb(events_dfl_poll_msecs, &disk_events_dfl_poll_msecs_param_ops,
		&disk_events_dfl_poll_msecs, 0644);

/*
 * disk_{alloc|add|del|release}_events - initialize and destroy disk_events.
 */
static void disk_alloc_events(struct gendisk *disk)
{
	struct disk_events *ev;

	if (!disk->fops->check_events)
		return;

	ev = kzalloc(sizeof(*ev), GFP_KERNEL);
	if (!ev) {
		pr_warn("%s: failed to initialize events\n", disk->disk_name);
		return;
	}

	INIT_LIST_HEAD(&ev->node);
	ev->disk = disk;
	spin_lock_init(&ev->lock);
	mutex_init(&ev->block_mutex);
	ev->block = 1;
	ev->poll_msecs = -1;
	INIT_DELAYED_WORK(&ev->dwork, disk_events_workfn);

	disk->ev = ev;
}

static void disk_add_events(struct gendisk *disk)
{
	if (!disk->ev)
		return;

	/* FIXME: error handling */
	if (sysfs_create_files(&disk_to_dev(disk)->kobj, disk_events_attrs) < 0)
		pr_warn("%s: failed to create sysfs files for events\n",
			disk->disk_name);

	mutex_lock(&disk_events_mutex);
	list_add_tail(&disk->ev->node, &disk_events);
	mutex_unlock(&disk_events_mutex);

	/*
	 * Block count is initialized to 1 and the following initial
	 * unblock kicks it into action.
	 */
	__disk_unblock_events(disk, true);
}

static void disk_del_events(struct gendisk *disk)
{
	if (!disk->ev)
		return;

	disk_block_events(disk);

	mutex_lock(&disk_events_mutex);
	list_del_init(&disk->ev->node);
	mutex_unlock(&disk_events_mutex);

	sysfs_remove_files(&disk_to_dev(disk)->kobj, disk_events_attrs);
}

static void disk_release_events(struct gendisk *disk)
{
	/* the block count should be 1 from disk_del_events() */
	WARN_ON_ONCE(disk->ev && disk->ev->block != 1);
	kfree(disk->ev);
}

#ifdef CONFIG_KDEBUGD_COUNTER_MONITOR

void sec_diskusage_update(struct sec_diskdata *pdisk_data)
{
       struct class_dev_iter iter;
       struct device *dev;

       mutex_lock(&block_class_lock);
       class_dev_iter_init(&iter, &block_class, NULL, &disk_type);
       while ((dev = class_dev_iter_next(&iter))) {
		struct gendisk *disk = dev_to_disk(dev);

		BUG_ON(!disk);
		sec_diskstats_dump_entry(disk, pdisk_data);
       }
       class_dev_iter_exit(&iter);
       mutex_unlock(&block_class_lock);
}
#endif

#ifdef CONFIG_BOOTPROFILE
/* Boot Chart  */

/**********************************************************************/
/*                                                                    */
/*     sec boot profile collection defines and declarations           */
/*                                                                    */
/**********************************************************************/

#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/kernel_stat.h>
#include <linux/kthread.h>
#include <linux/version.h>
#include <linux/utsname.h>
#include <linux/init.h>
#include <linux/cpumask.h>


/* Boot Profile states */

#define SEC_BP_STATE_UNINITED     0  /* not running, no memory
				      * allocated */
#define SEC_BP_STATE_STARTING     1  /* starting, preparing for
				      * sampling */
#define SEC_BP_STATE_RUNNING      2  /* running, sample collection
				      * started */
#define SEC_BP_STATE_COMPLETING   3  /* closing and preparing for
				      * further actions */
#define SEC_BP_STATE_COMPLETED    4  /* sampling done, profile data is
				      * available in previously
				      * allocated memory*/


/* Change it to enable / disable the debug trace prints for boot
 * profile solution.  It is only alocal setting for debugging. */
#define SEC_BP_TRACE_ON    0

/* Optimize the memory usage, calculations are done in the kernel code
 * and summarized results are prepared for report, e.g. all diskstats
 * can be summed up and just one entry per sample can be put in output
 * report */
#define SEC_DS_USE_SUMMARY  0

#if (SEC_BP_TRACE_ON == 0)
#define SEC_BP_TRACE(fmt...) do { } while (0)
#else
#define SEC_BP_TRACE (fmt, args...) do {		\
	printk(KERN_INFO "[BP] %s: " fmt , __FUNCTION__ , ##args); \
} while (0)
#endif /* #if (SEC_BP_TRACE_ON == 0) */


#define SEC_BP_COMM_NAME  "boot_profile"

#define SEC_BP_START_DELAY_MS  200 /* initial start delay ms */
#define SEC_BP_SAMPLE_INTERVAL_MS     200  /* sampling interval ms */

#define SEC_BP_NR_SAMPLES    240  /* at 0.2 sec sampling interval it
				     would cover 48 seconds */

#if (SEC_DS_USE_SUMMARY == 0)
/* assume 40 diskstats lines of entries per sample */
#define SEC_DS_NR_ENTRIES    (SEC_BP_NR_SAMPLES * 40)
#else
/* only 1 summarized entry is used */
#define SEC_DS_NR_ENTRIES    (SEC_BP_NR_SAMPLES)
#endif /* #if (SEC_DS_USE_SUMMARY == 0) */

/* assume on average 64 threads per sample */
#define SEC_PS_NR_ENTRIES (SEC_BP_NR_SAMPLES * 128)

#define PROC_STAT_LOG       "proc_stat.log"
#define PROC_PS_LOG         "proc_ps.log"
#define PROC_DISKSTATS_LOG  "proc_diskstats.log"

#define SEC_BP_DIRECT_DUMP           0

#if (SEC_BP_DIRECT_DUMP != 0)
#define SEC_BP_DIRECT_DUMP_START_DELAY_MS  6000
#endif
/* for slowing down the serial output of printk when directly dumping
 * the report on console */
#define DIRECT_DUMP_SLOW_MS  10

#define SEC_BP_LOG_DONE_COUNT   3 /* 3 types of logs are collected
				   * during sampling */
#define SEC_BP_WAIT_UPTIME_SEC  0 /* start profiling at the specified
				   * uptime */

#define SEC_BP_PROC_MIN_READ_LEN  (128 + 64) /* minimum supported proc
					      * read len */
#define SEC_BP_WARN_SMALL_BUF_LEN  0

/* For 'who' entries while filling the report for proc read */
#define SEC_BP_FILL_HEADER      0
#define SEC_BP_FILL_CPUSTAT     1
#define SEC_BP_FILL_PROC_PS     2
#define SEC_BP_FILL_DISKSTATS   3
#define SEC_BP_FILL_DONE        4

/* used for creating the proc buffer (line wise) */
#define SEC_BP_LINE_CACHE_SIZE  1024
/* useful for matching uptime with normal application logs / trace prints */
#define SEC_BP_TRACE_UPTIME     1

#define SEC_BP_COMM_WITH_PID    0
#if (SEC_BP_COMM_WITH_PID != 0)
#define SEC_BP_PROC_PS_FMT_STR  "%d (%s-%u) %c %d 0 0 0 0 0 0 " \
	"0 0 0 %lu %lu 0 0 0 0 "        \
"0 0 %llu 0 0 0 0 0 0 0 "       \
"0 0 0 0 0 0 0 %u 0 0 0 0 0\n"
#else
#define SEC_BP_PROC_PS_FMT_STR  "%d (%s) %c %d 0 0 0 0 0 0 "    \
	"0 0 0 %lu %lu 0 0 0 0 "        \
"0 0 %llu 0 0 0 0 0 0 0 "       \
"0 0 0 0 0 0 0 %u 0 0 0 0 0\n"
#endif

#define BOOTCHART_VER_STR   "4.0.0"
#define BOOTSTATS_HEADER_SIZE  512


struct sec_uptime_table {
	unsigned long uptime;
	int start_idx;
};

struct sec_cpustats {
	unsigned long uptime; /* uptime in units of 1/100sec*/
	u32 user; /* cputime64_t: only 32 bit value is stored, enough for
		   * 462 days = (4000000000/(100 * 60 * 60 * 24))*/
	u32 nice;
	u32 system;
	u32 idle;
	u32 iowait;
	u32 irq;
	u32 softirq;
	u32 steal;
	u32 user_rt;
	u32 system_rt;
};

struct sec_process_stat {
	pid_t pid;
	char tcomm[FIELD_SIZEOF(struct task_struct, comm)];
	char state;
	pid_t ppid;
	cputime_t utime;
	cputime_t stime;
	unsigned long cpuid;      /* for multicore cpuid */
	unsigned long start_time; /* 32-bit value would not wrap till 462
				   * days */
};

/* BDEVNAME_SIZE is currently 32, we have only small device names, so
 * we can save some space */
#define SEC_DS_BDEVNAME  12

/* stores the disk-stats required for disk utilization and throughput
 * along with the actual jiffy at that moment. */
struct sec_ds_struct {
	/* As we just store the summarized result, we can pretend it to be
	 * a dummy device (just for reporting), so generate the dummy name
	 * during report generation, no need to store here in:
	 * sec_ds_table[sec_ds_idx].name */
#if (SEC_DS_USE_SUMMARY == 0)
	char name[SEC_DS_BDEVNAME];
#endif
	long nsect_read;
	long nsect_write;
	unsigned io_ticks;
};

static int sec_bp_state = SEC_BP_STATE_UNINITED;

static struct sec_cpustats *sec_cs_table[CONFIG_NR_CPUS];
/* for second cpu Multi core */
/* static struct sec_cpustats* sec_cs_table2= NULL; */
static int sec_cs_idx;

static struct sec_process_stat *sec_ps_table;
static int sec_ps_idx;
static struct sec_uptime_table *sec_ps_ut_table ;
static int sec_ps_ut_idx;

static struct sec_ds_struct *sec_ds_table;
static int sec_ds_idx;
static struct sec_uptime_table *sec_ds_ut_table;
static int sec_ds_ut_idx;

#if (SEC_DS_USE_SUMMARY != 0)
static long ds_sum_nsect_read;
static long ds_sum_nsect_write;
static unsigned ds_sum_io_ticks;
#endif /* #if (SEC_DS_USE_SUMMARY != 0) */

static char *bs_header;

/* added in arch/arm/kernel/setup.c */
extern const char *bc_get_cpu_name(void);

static void sec_bp_start__(void);
static inline void sec_bp_reset(void);
static int sec_bp_alloc(void);
static void sec_bp_dealloc(void);
static int sec_bp_thread(void *arg);
#if (SEC_BP_WAIT_UPTIME_SEC != 0)
static inline void sec_uptime_wait(int nsec);
#endif

static inline int sec_bp_ds_filter(const char *str);
static void sec_bp_log_header(void);
static int sec_bp_log_cpu_stat(void);
static int sec_bp_log_process_stat(void);
static int sec_bp_log_diskstats(void);
static inline int sec_bp_log_ds_entry(struct gendisk *gd, struct disk_part_iter *piter);
static void sec_bp_meminfo_dump(void);

static void sec_bp_direct_dump(void);
static void sec_bp_dump_header(void);
static void sec_bp_dump_cpu_stat(void);
static void sec_bp_dump_process_stat(void);
static void sec_bp_dump_diskstats(void);

static int save_to_usb(void);

static inline void sec_bp_state_change(int new_state)
{
	SEC_BP_TRACE("state_change: %d -> %d\n", sec_bp_state, new_state);
	sec_bp_state = new_state;
}

/**********************************************************************
 *
 *  sec_bootstats:  proc related implementation.
 *
 **********************************************************************/

struct bp_stats_proc_filled_info {
	int who; /* 0= header, 1= cpu_stat, 2= process_stat, 3= diskstats,
		    other_value= done */
	int fname_is_done; /* #@fname printed before the stats */
	int next_idx; /* Any value out of range, i.e. < 0 or > available
		       * means end of data. starts with 0. */
	int next_sub_idx; /* index of sub table, if any. */
	int next_idx_is_done;
	off_t next_expected_seq_offset;
	int is_no_more_data;
	char *line_cache;
};

static struct bp_stats_proc_filled_info bp_stats_proc_read_ctx;

static int sec_bp_proc_fill_report(char *buffer, off_t offset,
		int buffer_length);
static int sec_bp_proc_fill_header(char *buffer, int buffer_length);
static int sec_bp_proc_fill_cpu_stat(char *buffer, int buffer_length);
static int sec_bp_proc_fill_process_stat(char *buffer, int buffer_length);
static int sec_bp_proc_fill_diskstats(char *buffer, int buffer_length);

static inline int sec_bp_proc_check_length(int buffer_length);

/*Fucntion definition written in fs/proc.c */
extern const char *sec_bp_get_task_state(struct task_struct *tsk);


/**********************************************************************
 *                                                                    *
 *              proc related functions                                *
 *                                                                    *
 **********************************************************************/
static void sec_bp_proc_activate(void);

/* for /proc/bootstats writing */
static int sec_bp_proc_write(struct file *file, const char *buffer,
		unsigned long count, void *data);

/* for /proc/bootstats reading */
static int sec_bp_proc_read(char *buffer, char **buffer_location,
		off_t offset, int buffer_length, int *eof,
		void *data);

/* Filter the device names that should be accounted for disk-stats
 * For Chelsea:
 *
 *   skip:     ram0 ... ram 15
 *   consider: tbmlc, tbml1 ... tbml13
 *   consider: bml0/c, bml1 ... bml13
 *   consider: stl1 ... stl13
 *   consider: sda, sdb, sdc, ...
 *   skip:     sda1, sdb1, ...
 *   consider: hda, hdb (hard-disk?)
 */
static inline int sec_bp_ds_filter(const char *str)
{
	/* It is the support for all kind of device */
	return 1;
	/* To provide the support for very precise devices to save memory
	 *  for bootchart*/
	/* just do a quick check */
}


/**********************************************************************
 * some user interace functions (called writing of /proc/bootstats)
 **********************************************************************/

static void sec_bp_print_help(void);
static void sec_bp_print_status(void);
static inline void sec_bp_start(void);


/**********************************************************************/
/*                                                                    */
/*     sec boot profile functions definitions                         */
/*                                                                    */
/**********************************************************************/

void sec_bp_start__(void)
{
	pid_t pid = 0;
	struct task_struct *sec_bp_tsk = NULL;

	SEC_BP_TRACE("enter\n");
	BUG_ON(sec_bp_state == SEC_BP_STATE_STARTING);
	BUG_ON(sec_bp_state == SEC_BP_STATE_RUNNING);
	sec_bp_state_change(SEC_BP_STATE_STARTING);

	/* changing state to running is done by thread function, if
	 * success in creating thread */
	sec_bp_tsk = kthread_create(sec_bp_thread, NULL, "bp_thread");
	if (IS_ERR(sec_bp_tsk)) {
		sec_bp_tsk = NULL;
		printk("Failed: bp_thread Thread Creation\n");
		return;
	}

	/* update task flag and wakeup process */
	sec_bp_tsk->flags |= PF_NOFREEZE;
	wake_up_process(sec_bp_tsk);

	pid = sec_bp_tsk->pid;

	if (likely(pid >= 0)) {
		read_lock(&tasklist_lock);
		sec_bp_tsk = find_task_by_pid_ns(pid, &init_pid_ns);
		if (likely(sec_bp_tsk)) {
			task_lock(sec_bp_tsk);
			strncpy(sec_bp_tsk->comm, SEC_BP_COMM_NAME, sizeof(sec_bp_tsk->comm));
			sec_bp_tsk->comm[sizeof(sec_bp_tsk->comm) - 1] = '\0'; /* for boundary cond. */
			printk("--- [BP] --- pid= %u: " SEC_BP_COMM_NAME
					" task= (%.20s)\n", pid, sec_bp_tsk->comm);
			task_unlock(sec_bp_tsk);
		}
		read_unlock(&tasklist_lock);
	} else {
		BUG_ON(sec_bp_state != SEC_BP_STATE_STARTING);
		sec_bp_state_change(SEC_BP_STATE_UNINITED);
	}

	SEC_BP_TRACE("pid= %u\n", pid);
}

/* Note: There is a potential race condition of proc being read
 * when profiling is re-started.  It is not supported to
 * simultaneous read the proc and start the profiling in parallel
 * when proc read is in happening. */
void sec_bp_reset(void)
{
	BUG_ON(sec_bp_state != SEC_BP_STATE_STARTING);

	sec_cs_idx = 0;
	sec_ps_idx = 0;
	sec_ps_ut_idx = 0;
	sec_ds_idx = 0;
	sec_ds_ut_idx = 0;

#if (SEC_DS_USE_SUMMARY != 0)
	ds_sum_nsect_read = 0;
	ds_sum_nsect_write = 0;
	ds_sum_io_ticks = 0;
#endif /* #if (SEC_DS_USE_SUMMARY != 0) */

	memset(&bp_stats_proc_read_ctx, 0, sizeof(bp_stats_proc_read_ctx));
}

int sec_bp_alloc(void)
{
	int ret = 1;
	int len = 0;
	unsigned int i;

	SEC_BP_TRACE("enter\n");
	BUG_ON(sec_bp_state != SEC_BP_STATE_STARTING);

	/* cpu stat */
	/* Second CPU for multicore */
	for (i = 0; i < CONFIG_NR_CPUS; i++) {
		if (!sec_cs_table[i]) {
			len = (SEC_BP_NR_SAMPLES
					* sizeof(struct sec_cpustats));
			sec_cs_table[i] = vmalloc(len);
			if (!sec_cs_table[i]) {
				printk(KERN_WARNING "%s: cpustat: memory alloc error\n",
						__FUNCTION__);
				goto alloc_out;
			}
			memset(sec_cs_table[i], 0, len);
		}
	}
	/* process stat */
	if (!sec_ps_ut_table) {
		len = (SEC_BP_NR_SAMPLES
				* sizeof(*sec_ps_ut_table));
		sec_ps_ut_table = vmalloc(len);
		if (!sec_ps_ut_table) {
			printk(KERN_WARNING "%s: process_stat_ut: memory alloc error\n",
					__FUNCTION__);
			goto alloc_out;
		}
		memset(sec_ps_ut_table, 0, len);
	}

	if (!sec_ps_table) {
		len = (SEC_PS_NR_ENTRIES
				* sizeof(*sec_ps_table));
		sec_ps_table = vmalloc(len);
		if (!sec_ps_table) {
			printk(KERN_WARNING "%s: process_stat: memory alloc error\n",
					__FUNCTION__);
			goto alloc_out;
		}
		memset(sec_ps_table, 0, len);
	}

	/* disk stat */
	if (!sec_ds_ut_table) {
		len = (SEC_BP_NR_SAMPLES
				* sizeof(*sec_ds_ut_table));
		sec_ds_ut_table = vmalloc(len);
		if (!sec_ds_ut_table) {
			printk(KERN_WARNING "%s: diskstats_ut: memory alloc error\n",
					__FUNCTION__);
			goto alloc_out;
		}
		memset(sec_ds_ut_table, 0, len);
	}

	if (!sec_ds_table) {
		len = (SEC_DS_NR_ENTRIES
				* sizeof(*sec_ds_table));
		sec_ds_table = vmalloc(len);
		if (!sec_ds_table) {
			printk(KERN_WARNING "%s: diskstats: memory alloc error\n",
					__FUNCTION__);
			goto alloc_out;
		}
		memset(sec_ds_table, 0, len);
	}

	ret = 0; /* alloc success */
alloc_out:

	SEC_BP_TRACE("exit: ret= %d\n", ret);
	return ret;
}

void sec_bp_dealloc(void)
{
	unsigned int i;
	SEC_BP_TRACE("vfree table\n");
	printk("[BP] %s: releasing memory\n", __FUNCTION__);

	BUG_ON(sec_bp_state != SEC_BP_STATE_COMPLETED);

	/* Note: vfree(addr) performs no-operation when 'addr' is NULL. */

	for (i = 0; i < CONFIG_NR_CPUS; i++) {
		if (sec_cs_table[i]) {
			vfree(sec_cs_table[i]);
			sec_cs_table[i] = NULL;
		}
	}

	if (sec_ps_table) {
		vfree(sec_ps_table);
		sec_ps_table = NULL;
	}

	if (sec_ps_ut_table) {
		vfree(sec_ps_ut_table);
		sec_ps_ut_table = NULL;
	}

	if (sec_ds_table) {
		vfree(sec_ds_table);
		sec_ds_table = NULL;
	}

	if (sec_ds_ut_table) {
		vfree(sec_ds_ut_table);
		sec_ds_ut_table = NULL;
	}

	if (bs_header) {
		vfree(bs_header);
		bs_header = NULL;
	}
	sec_bp_state_change(SEC_BP_STATE_UNINITED);
}

static int sec_bp_thread(void *arg)
{
	int ret = 0;
	int is_done = 0;

	SEC_BP_TRACE("enter\n");
	BUG_ON(sec_bp_state != SEC_BP_STATE_STARTING);

	/* delay before start collecting the stats */
#if (SEC_BP_START_DELAY_MS != 0)
	msleep(SEC_BP_START_DELAY_MS);
#endif

	sec_bp_reset();
	SEC_BP_TRACE("alloc memory\n");
	ret = sec_bp_alloc();

	if (ret) {
		printk(KERN_WARNING "%s: memory alloc failure\n", __FUNCTION__);
		sec_bp_state_change(SEC_BP_STATE_COMPLETED); /* transition state */
		SEC_BP_TRACE("deallocing memory\n");
		sec_bp_dealloc();
		return 1;
	}

#if (SEC_BP_WAIT_UPTIME_SEC != 0)
	sec_uptime_wait(SEC_BP_WAIT_UPTIME_SEC);
#endif

	for (; !is_done; ) {

		msleep(SEC_BP_SAMPLE_INTERVAL_MS);
		is_done = sec_bp_log_cpu_stat();
		is_done += sec_bp_log_process_stat();
		is_done += sec_bp_log_diskstats();

		/* continue till all are done */
		is_done = (is_done == SEC_BP_LOG_DONE_COUNT) ? 1 : 0;
	}

	sec_bp_log_header();
	sec_bp_state_change(SEC_BP_STATE_COMPLETING); /* transition state */

#if (SEC_BP_DIRECT_DUMP != 0)
	SEC_BP_TRACE("delay for direct dump: sleeping %dms\n",
			SEC_BP_DIRECT_DUMP_START_DELAY_MS);

	/* Dump the collected log directly to console.  Useful for
	 * testing.  The reason for delay is that we can choose suitable
	 * moment, to avoid mixing with normal application power up
	 * logs. */
	msleep(SEC_BP_DIRECT_DUMP_START_DELAY_MS);

	SEC_BP_TRACE("direct dump\n");
	sec_bp_direct_dump();
#endif /* #if (SEC_BP_DIRECT_DUMP != 0) */

	/* create proc (first time) / reset proc for new data */
	sec_bp_proc_activate();
	sec_bp_meminfo_dump();

	sec_bp_state_change(SEC_BP_STATE_COMPLETED);

	printk("%s: ------------------------------------\n", __FUNCTION__);
	printk("%s: exiting the sec bootprof thread func\n", __FUNCTION__);
	printk("%s: ------------------------------------\n", __FUNCTION__);
	SEC_BP_TRACE("exit\n");
	printk("Saving to usb.....................\n");

	msleep(5000);
	printk("Saving to usb....\n");
	if (!save_to_usb())
		printk("USB write bootstats file fail\n");
	else
		printk("Saving to usb complete!\n");

	return 0;
}


/**********************************************************************
 *
 *  Boot profile information collection related functions.
 *
 **********************************************************************/

void sec_bp_log_header(void)
{
	if (!bs_header) {
		bs_header = vmalloc(BOOTSTATS_HEADER_SIZE);
		if (!bs_header) {
			printk (KERN_ERR "%s: vmalloc failed\n", __FUNCTION__);
			return;
		}
	}

	snprintf(bs_header, BOOTSTATS_HEADER_SIZE,
			"#@header\n"
			"version = " BOOTCHART_VER_STR "\n"
			"title = Boot chart for %s (Sun May 31 21:01:58 IST 2009)\n"
			"system.uname = %s %s %s %s\n"
			"system.release = %s\n"
			"system.cpu = model name	: %s (%u)\n"
			"system.kernel.options = %s\n",
			init_uts_ns.name.nodename, init_uts_ns.name.sysname,
			init_uts_ns.name.release, init_uts_ns.name.version,
			init_uts_ns.name.machine, init_uts_ns.name.release,
			bc_get_cpu_name(), num_online_cpus(),
			saved_command_line);
}

int sec_bp_log_cpu_stat(void)
{
	int i;
	cputime64_t user_rt, user, nice, system_rt, system, idle,
		    iowait, irq, softirq, steal;
	/* second cpu */
	struct timespec uptime;

	SEC_BP_TRACE("enter sec_bp_log_cpu_stat\n");
	if (sec_cs_idx >= SEC_BP_NR_SAMPLES)
		return 1;

	user_rt = user = nice = system_rt = system = idle = iowait =
		irq = softirq = steal = cputime64_zero;

	do_posix_clock_monotonic_gettime(&uptime);

	for_each_online_cpu(i) {
		/* Getting CPU state for first core */
		user = cputime64_add(user, kstat_cpu(i).cpustat.user);
		nice = cputime64_add(nice, kstat_cpu(i).cpustat.nice);
		system = cputime64_add(system, kstat_cpu(i).cpustat.system);
		idle = cputime64_add(idle, kstat_cpu(i).cpustat.idle);
		iowait = cputime64_add(iowait, kstat_cpu(i).cpustat.iowait);
		irq = cputime64_add(irq, kstat_cpu(i).cpustat.irq);
		softirq = cputime64_add(softirq, kstat_cpu(i).cpustat.softirq);
		steal = cputime64_add(steal, kstat_cpu(i).cpustat.steal);


		sec_cs_table[i][sec_cs_idx].uptime
			= ((unsigned long) uptime.tv_sec * 100
					+ uptime.tv_nsec / (NSEC_PER_SEC / 100));
		sec_cs_table[i][sec_cs_idx].user = (u32) user;
		sec_cs_table[i][sec_cs_idx].nice = (u32) nice;
		sec_cs_table[i][sec_cs_idx].system = (u32) system;
		sec_cs_table[i][sec_cs_idx].idle = (u32) idle;
		sec_cs_table[i][sec_cs_idx].iowait = (u32) iowait;
		sec_cs_table[i][sec_cs_idx].irq = (u32) irq;
		sec_cs_table[i][sec_cs_idx].softirq = (u32) softirq;
		sec_cs_table[i][sec_cs_idx].steal = (u32) steal;
		sec_cs_table[i][sec_cs_idx].user_rt = 0;
		sec_cs_table[i][sec_cs_idx].system_rt = 0;

		user_rt = user = nice = system_rt = system = idle = iowait =
			irq = softirq = steal = cputime64_zero;

	}
#if (SEC_BP_TRACE_UPTIME != 0)
	printk("--- [BP %lu] ---\n", sec_cs_table[0][sec_cs_idx].uptime);
#endif
	++sec_cs_idx;

	SEC_BP_TRACE("normal exit\n");
	return 0;
}

/* Note:
 * ====
 * We should find out why we require tty_mutex lock.
 * Also for task->group_leader is tty_mutex lock used?
 * Find if group_leader is protected by task_lock or tty_mutex.
 */
static int sec_bp_log_process_stat(void)
{
	struct task_struct *g = NULL;
	struct task_struct *p = NULL;
	unsigned long long start_time = 0;
	struct timespec uptime;

	SEC_BP_TRACE("enter\n");

	if (sec_ps_ut_idx >= SEC_BP_NR_SAMPLES
			|| sec_ps_idx >= SEC_PS_NR_ENTRIES)
		return 1;

	read_lock(&tasklist_lock);

	do_posix_clock_monotonic_gettime(&uptime);
	sec_ps_ut_table[sec_ps_ut_idx].start_idx
		= sec_ps_idx;

	do_each_thread(g, p) {
		sec_ps_table[sec_ps_idx].pid = p->pid;
		/* multicore: storing core id in sec_ps_table */
		sec_ps_table[sec_ps_idx].cpuid = task_cpu(p);
		/* multicore end */

		task_lock(p);
		strncpy(sec_ps_table[sec_ps_idx].tcomm,
				p->comm, sizeof(sec_ps_table[sec_ps_idx].tcomm));
		sec_ps_table[sec_ps_idx].tcomm[
			sizeof(sec_ps_table[sec_ps_idx].tcomm) - 1] = '\0';
		task_unlock(p);

		sec_ps_table[sec_ps_idx].state =
			*sec_bp_get_task_state(p);
		sec_ps_table[sec_ps_idx].ppid =
			(pid_alive(p) ? p->group_leader->real_parent->tgid : 0);
		sec_ps_table[sec_ps_idx].utime = p->utime;
		sec_ps_table[sec_ps_idx].stime = p->stime;

		/* Temporary variable needed for gcc-2.96 */
		/* convert timespec -> nsec*/
		start_time = (unsigned long long)p->start_time.tv_sec * NSEC_PER_SEC
			+ p->start_time.tv_nsec;
		/* convert nsec -> ticks */
		start_time = nsec_to_clock_t(start_time);
		sec_ps_table[sec_ps_idx].start_time = start_time;
		++sec_ps_idx;
		if (sec_ps_idx >= SEC_PS_NR_ENTRIES) {
			SEC_BP_TRACE("limit reached sec_ps_idx= %d\n", sec_ps_idx);
			goto thread_out; /* break out of nested loop */
		}
	} while_each_thread(g, p);

thread_out:

	sec_ps_ut_table[sec_ps_ut_idx].uptime
		= ((unsigned long) uptime.tv_sec * 100
				+ uptime.tv_nsec / (NSEC_PER_SEC / 100));
	++sec_ps_ut_idx;

	read_unlock(&tasklist_lock);
	/* mutex_unlock(&tty_mutex); */

	SEC_BP_TRACE("normal exit\n");
	return 0;
}

/* Note:
 * we can try to check if we can use jiffies as uptime (then need to
 * start with 0 or some near to 0 value, otherwise BootChart will show
 * misleading timeline). */
static int sec_bp_log_diskstats(void)
{
	struct class_dev_iter iter;
	struct device *dev;
	struct gendisk *disk = NULL;
	struct timespec uptime;
	struct disk_part_iter piter;

	SEC_BP_TRACE("enter\n");

	if (sec_ds_ut_idx >= SEC_BP_NR_SAMPLES
			|| sec_ds_idx >= SEC_DS_NR_ENTRIES)
		return 1;

#if (SEC_DS_USE_SUMMARY != 0)
	ds_sum_nsect_read = 0;
	ds_sum_nsect_write = 0;
	ds_sum_io_ticks = 0;
#endif

	sec_ds_ut_table[sec_ds_ut_idx].start_idx = sec_ds_idx;
	/* mutex_lock(&block_class_lock); */
	do_posix_clock_monotonic_gettime(&uptime);

	class_dev_iter_init(&iter, &block_class, NULL, &disk_type);
	while ((dev = class_dev_iter_next(&iter)))	{
		disk = dev_to_disk(dev);
		BUG_ON(!disk);
		/* disk_part_iter_init(&piter, disk, DISK_PITER_INCL_EMPTY_PART0); */
		disk_part_iter_init(&piter, disk, DISK_PITER_INCL_PART0);
		while (sec_bp_log_ds_entry(disk, &piter)) {
			if (sec_ds_idx >= SEC_DS_NR_ENTRIES) {
				SEC_BP_TRACE("limit reached sec_ds_ut_idx= %d\n", sec_ds_idx);
				break;
			}
		}
		disk_part_iter_exit(&piter);

	}
	class_dev_iter_exit(&iter);

	/* mutex_unlock(&block_class_lock); */

#if (SEC_DS_USE_SUMMARY != 0)
	sec_ds_table[sec_ds_idx].nsect_read
		= ds_sum_nsect_read;
	sec_ds_table[sec_ds_idx].nsect_write
		= ds_sum_nsect_write;
	sec_ds_table[sec_ds_idx].io_ticks
		= ds_sum_io_ticks;
	++sec_ds_idx;
#endif

	sec_ds_ut_table[sec_ds_ut_idx].uptime
		= ((unsigned long) uptime.tv_sec * 100
				+ uptime.tv_nsec / (NSEC_PER_SEC / 100));
	++sec_ds_ut_idx;

	SEC_BP_TRACE("normal exit\n");
	return 0;
}

static inline int sec_bp_log_ds_entry(struct gendisk *gd, struct disk_part_iter *piter)
{
	char buf[BDEVNAME_SIZE];
	struct hd_struct *hd;
	int cpu;

	SEC_BP_TRACE("enter\n");

	SEC_BP_TRACE("preempt_disable()\n");
	/* preempt_disable(); */
	hd = disk_part_iter_next(piter);
	if (hd) {
		cpu = part_stat_lock();
		part_round_stats(cpu, hd);
		part_stat_unlock();
	} else {
		return 0;
	}
	/* preempt_enable(); */
	SEC_BP_TRACE("preempt_enable()\n");

	/* limit of input char array is properly handled in disk_name(),
	 * so no need to check overlow case */
	disk_name(gd, hd->partno, buf);

	/* check if does not need to be considered for disk-stats */
	if (sec_bp_ds_filter(buf) && hd) {
#if (SEC_DS_USE_SUMMARY != 0)
		ds_sum_nsect_read += part_stat_read(hd, sectors[0]);
		ds_sum_nsect_write += part_stat_read(hd, sectors[1]);
		ds_sum_io_ticks += part_stat_read(hd, io_ticks);
#else
		strncpy(sec_ds_table[sec_ds_idx].name,
				buf, SEC_DS_BDEVNAME);
		sec_ds_table[sec_ds_idx].name[SEC_DS_BDEVNAME - 1] = '\0';

		sec_ds_table[sec_ds_idx].nsect_read
			= part_stat_read(hd, sectors[0]);

		sec_ds_table[sec_ds_idx].nsect_write
			= part_stat_read(hd, sectors[1]);

		sec_ds_table[sec_ds_idx].io_ticks
			= part_stat_read(hd, io_ticks);
		++sec_ds_idx;
#endif /* #if (SEC_DS_USE_SUMMARY == 0) */
	}
	return 1;
}


/**********************************************************************
 *
 *  Dump boot profile information to console related functions.
 *
 **********************************************************************/


/* Dump the memory used and actually allocated memory by the internal
 * tables of boot profile soluton.  It is only for debugging and
 * information. */
static void sec_bp_meminfo_dump(void)
{
	const int cpu_used_mem = (sec_cs_idx
			* sizeof(struct sec_cpustats)*CONFIG_NR_CPUS);

	const int cpu_alloc_mem = (SEC_BP_NR_SAMPLES
			* sizeof(struct sec_cpustats)*CONFIG_NR_CPUS);

	const int ps_ut_used_mem = (sec_ps_ut_idx
			* sizeof(*sec_ps_ut_table));
	const int ps_ut_alloc_mem = (SEC_BP_NR_SAMPLES
			* sizeof(*sec_ps_ut_table));
	const int ps_used_mem = (sec_ps_idx
			* sizeof(*sec_ps_table));
	const int ps_alloc_mem = (SEC_PS_NR_ENTRIES
			* sizeof(*sec_ps_table));

	const int ds_ut_used_mem = (sec_ds_ut_idx
			* sizeof(*sec_ds_ut_table));
	const int ds_ut_alloc_mem = (SEC_BP_NR_SAMPLES
			* sizeof(*sec_ds_ut_table));
	const int ds_used_mem = (sec_ds_idx
			* sizeof(*sec_ds_table));
	const int ds_alloc_mem = (SEC_DS_NR_ENTRIES
			* sizeof(*sec_ds_table));

	BUG_ON(sec_bp_state != SEC_BP_STATE_COMPLETING
			&& sec_bp_state != SEC_BP_STATE_COMPLETED);

	printk("\n");
	printk("#   boot prof meminfo\n");
	printk("#\n");
	printk("# used memory   /  allocated memory\n");
	printk("# cpustat memory:  %d x %d = %d / %d\n",
			sec_cs_idx, sizeof(struct sec_cpustats),
			cpu_used_mem, cpu_alloc_mem);

	printk("# process stat ut memory:  %d x %d = %d / %d\n",
			sec_ps_ut_idx, sizeof(*sec_ps_ut_table),
			ps_ut_used_mem, ps_ut_alloc_mem);

	printk("# process stat memory: %d x %d = %d / %d\n",
			sec_ps_idx, sizeof(*sec_ps_table),
			ps_used_mem, ps_alloc_mem);

	printk("# diskstats ut memory: %d x %d = %d / %d\n",
			sec_ds_ut_idx, sizeof(*sec_ds_ut_table),
			ds_ut_used_mem, ds_ut_alloc_mem);

	printk("# diskstats memory: %d x %d = %d / %d\n",
			sec_ds_idx, sizeof(*sec_ds_table),
			ds_used_mem, ds_alloc_mem);

	printk("# Total memory: %d / %d\n",
			(cpu_used_mem + ps_ut_used_mem + ps_used_mem
			 + ds_ut_used_mem + ds_used_mem),
			(cpu_alloc_mem + ps_ut_alloc_mem + ps_alloc_mem
			 + ds_ut_alloc_mem + ds_alloc_mem));

	printk("#\n");
	printk("\n");
}

static void sec_bp_direct_dump(void)
{
	SEC_BP_TRACE("enter\n");

	BUG_ON(sec_bp_state != SEC_BP_STATE_COMPLETING
			&& sec_bp_state != SEC_BP_STATE_COMPLETED);

	sec_bp_dump_header();

	printk("\n");
	printk("# cpu stat start\n");
	printk("\n");
	sec_bp_dump_cpu_stat();

	printk("\n");
	printk("# process stat start\n");
	printk("\n");
	sec_bp_dump_process_stat();

	printk("\n");
	printk("# diskstats start\n");
	printk("\n");
	sec_bp_dump_diskstats();

	printk("\n");
	printk("# boot prof report end\n");
	printk("\n");

}

static void sec_bp_dump_header(void)
{
	printk("#   boot prof header\n");
	printk("#\n");
	printk("\n");
	if (bs_header)
		printk("%s", bs_header);
}

/* for /proc/stat */
static void sec_bp_dump_cpu_stat(void)
{
	int ii = 0;
	unsigned int i;

	BUG_ON(!sec_cs_table[0]);
	printk("\n#@%s\n", PROC_STAT_LOG);
	for (ii = 0; ii < sec_cs_idx; ++ii) {

		for_each_online_cpu(i)
		{
			printk("%lu\n", sec_cs_table[i][ii].uptime);

			printk("cpu%u  %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu\n", i,
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].user),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].nice),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].system),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].idle),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].iowait),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].irq),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].softirq),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].steal),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].user_rt),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].system_rt));
			printk("\n");
		}
	}
}


/* Note:
 * =====
 *
 *  Do we need to add the time of all the threads also for the
 *  main thread?  refer to array.c, I think it is not required.
 *  We should keep eye on 'whole' argument, when received as one
 *  in array.c function? */
static void sec_bp_dump_process_stat(void)
{
	int ii = 0;
	int jj = 0;
	int end_idx = 0;

	BUG_ON(!sec_ps_ut_table);
	BUG_ON(!sec_ps_table);

	printk("#@%s\n", PROC_PS_LOG);
	for (ii = 0; ii < sec_ps_ut_idx; ++ii) {

		printk("%lu\n", sec_ps_ut_table[ii].uptime);

		if (ii == sec_ps_ut_idx - 1)
			end_idx = sec_ps_idx; /* last record limit */
		else
			end_idx = sec_ps_ut_table[ii+1].start_idx;

		for (jj = sec_ps_ut_table[ii].start_idx; jj < end_idx; ++jj)
			printk(SEC_BP_PROC_PS_FMT_STR,
					sec_ps_table[jj].pid,
					sec_ps_table[jj].tcomm,
#if (SEC_BP_COMM_WITH_PID != 0)
					sec_ps_table[jj].pid, /* append to tcomm */
#endif
					/* sec_ps_table[jj].cpuid,*/ /* cpuid */
					sec_ps_table[jj].state,
					sec_ps_table[jj].ppid,
					cputime_to_clock_t(sec_ps_table[jj].utime),
					cputime_to_clock_t(sec_ps_table[jj].stime),
					(unsigned long long)sec_ps_table[jj].start_time,
					(unsigned int) sec_ps_table[jj].cpuid);

		printk("\n");
		msleep(DIRECT_DUMP_SLOW_MS);
	}
}

static void sec_bp_dump_diskstats(void)
{
	int ii = 0;
	int jj = 0;
	int end_idx = 0;

	BUG_ON(!sec_ds_ut_table);
	BUG_ON(!sec_ds_table);

	printk("#@%s\n", PROC_DISKSTATS_LOG);
	for (ii = 0; ii < sec_ds_ut_idx; ++ii) {

		printk("%lu\n", sec_ds_ut_table[ii].uptime);

		if (ii == sec_ds_ut_idx - 1)
			end_idx = sec_ds_idx; /* last record limit */
		else
			end_idx = sec_ds_ut_table[ii+1].start_idx;

		for (jj = sec_ds_ut_table[ii].start_idx; jj < end_idx; ++jj)
			/* major first_minor name ios[0] merges[0] nsect_read
			   ticks[0] ios[1] merges[1] nsect_read sect_write
			   ticks[1] in_flight io_ticks time_in_queue */
			/* "%4d %4d %s %lu %lu %llu %u %lu %lu %llu %u %u %u %u\n" */
			printk("0 0 %s 0 0 %llu 0 0 0 %llu 0 0 %u 0\n",
#if (SEC_DS_USE_SUMMARY == 0)
					sec_ds_table[jj].name,
#else
					"sum_bdev",
#endif
					(unsigned long long)sec_ds_table[jj].nsect_read,
					(unsigned long long)sec_ds_table[jj].nsect_write,
					jiffies_to_msecs(sec_ds_table[jj].io_ticks));
		printk("\n");
		msleep(DIRECT_DUMP_SLOW_MS);
	}
}

/**********************************************************************/
/*                                                                    */
/*     sec boot profile utility functions                             */
/*                                                                    */
/**********************************************************************/

/**********************************************************************
 *
 *  sec_bootstats:  proc related implementation.
 *
 **********************************************************************/

static void sec_bp_proc_activate(void)
{
	static struct proc_dir_entry *sec_bootstats_proc_dentry;
	SEC_BP_TRACE("enter\n");
	BUG_ON(sec_bp_state != SEC_BP_STATE_COMPLETING);

	if (!sec_bootstats_proc_dentry) {
		SEC_BP_TRACE("creating /proc/bootstats\n");
		sec_bootstats_proc_dentry = create_proc_entry("bootstats", 0, NULL);

		if (sec_bootstats_proc_dentry) {
			sec_bootstats_proc_dentry->read_proc  = sec_bp_proc_read;
			sec_bootstats_proc_dentry->write_proc = sec_bp_proc_write;
		} else
			printk(KERN_WARNING "/proc/bootstats creation failed\n");
	}
}

/* This function is called with the /proc file is written */
int sec_bp_proc_write(struct file *file, const char *buffer,
		unsigned long count, void *data)
{
	long ilength = -EFAULT;

	SEC_BP_TRACE("(file= %p, buffer= %p, count= %lu, data= %p)\n",
			file, buffer, count, data);

	(void) file; /* not used, well except for trace printks */
	(void) data; /* not used */

	/* we never expect it to be null, but put a condition to handle it
	   is release builds */
	BUG_ON(!buffer);

	if (!buffer) {
		printk (KERN_ERR "buffer is null\n");
		ilength =  -EFAULT;

	} else if (count > 0) {
		char cinput[4];

		ilength = (count >= sizeof(cinput) ? sizeof(cinput) - 1 : count);
		/* write data to the buffer */
		if (copy_from_user(cinput, buffer, ilength)) {
			printk(KERN_WARNING "%s: Error in copy_from_user: Returning\n",
					__FUNCTION__);
			return -EPERM;
		}

		cinput[ilength] = '\0';
		SEC_BP_TRACE("buf@3= (%s)\n", cinput);
		switch (cinput[0]) {
		case 'h':
			sec_bp_print_help();
			break;
		case '1':
			/* (re)start the profiling. */
			if (sec_bp_state == SEC_BP_STATE_UNINITED
					|| sec_bp_state == SEC_BP_STATE_COMPLETED) {
				printk(KERN_INFO "%s: Profiling OFF -> active\n", __FUNCTION__);
				sec_bp_start();
			} else {
				BUG_ON(sec_bp_state != SEC_BP_STATE_RUNNING
						&& sec_bp_state != SEC_BP_STATE_STARTING
						&& sec_bp_state != SEC_BP_STATE_COMPLETING);
				printk(KERN_INFO "%s: Profiling already active\n", __FUNCTION__);
			}
			break;
		case 'd':
			if (sec_bp_state == SEC_BP_STATE_COMPLETED)
				sec_bp_direct_dump();
			else
				printk(KERN_INFO "%s: not in finished state (%d)\n",
						__FUNCTION__, sec_bp_state);
			break;
		case 's':
			sec_bp_print_status();
			break;
		case 'c':
			if (sec_bp_state == SEC_BP_STATE_COMPLETED) {
				printk(KERN_INFO "%s: dealloc memory\n", __FUNCTION__);
				sec_bp_dealloc();
				BUG_ON(sec_bp_state != SEC_BP_STATE_UNINITED);
			} else if (sec_bp_state == SEC_BP_STATE_UNINITED) {
				printk(KERN_INFO "%s: memory is already released\n",
						__FUNCTION__);
			} else {
				BUG_ON(sec_bp_state != SEC_BP_STATE_STARTING
						&& sec_bp_state != SEC_BP_STATE_RUNNING
						&& sec_bp_state != SEC_BP_STATE_COMPLETING);
				printk(KERN_INFO "%s: ERR: currently running\n",
						__FUNCTION__);
			}

			break;
		default:
			printk(KERN_WARNING "%s: invalid choice [0x%x, %c]\n",
					__FUNCTION__, cinput[0], cinput[0]);
		}

		/* we need to pretend that we consumed all the buffer, so that
		 * the system feels ok and does not attempt to re-write
		 * again */
		ilength = count;

	} else {
		printk(KERN_WARNING "%s: insufficient data, count= %ld\n",
				__FUNCTION__, count);
		ilength = -EFAULT;
	}

	return ilength;
}

/* Called when /proc/bootstats is read.
 * Note:
 * It is not re-entrant function. Right now it does not require to be
 * re-entrant, unless some user tries to access it from two shells, or
 * more than one program / thread.  Do not access this module for
 * output report simultaneous from more than one place, e.g. kdebugd
 * and user or from more than one threads.
 *
 * We do not support a small buffer read, also we never support
 * seeking.  Users should read a big buffer say 512 or 1024 bytes and
 * typically a line size may be around 80 characters. We may actually
 * fill less data, in that case we return that as return value, but we
 * always expect a good size buffer. */
int sec_bp_proc_read(char *buffer, char **buffer_location,
		off_t offset, int buffer_length, int *eof,
		void *data)
{
	int len_copied = 0;
	int ret = -EFAULT;

	SEC_BP_TRACE("(buffer= %p, buffer_location= %p, "
			"offset= %ld, buffer_length= %d, eof= %p, data= %p)\n",
			buffer, buffer_location, offset, buffer_length, eof, data);

	/* It is required, otherwise for large proc data next read does
	   not happen with proper offset. */
	*buffer_location = buffer;
	(void) data; /* not used */

	if (sec_bp_state != SEC_BP_STATE_COMPLETED) {

		printk(KERN_WARNING "ERR: boot-profiling is "
				"not completed, state= %d\n", sec_bp_state);
		ret = -ENOENT;
		goto proc_read_out;

	} else if (!sec_bp_proc_check_length(buffer_length)) {
#if (SEC_BP_WARN_SMALL_BUF_LEN != 0)
		/* It clashes with normal prints. */
		printk(KERN_WARNING "++++++++++++++++++++++++++++++++++++++\n");
		printk(KERN_WARNING "======================================\n");
		printk(KERN_WARNING "buffer_length= %d:  "
				"small length not supported\n", buffer_length);
		printk(KERN_WARNING "======================================\n");
		printk(KERN_WARNING "++++++++++++++++++++++++++++++++++++++\n");
#endif
		ret = -ENOBUFS;
		goto proc_read_out;

	} else if (!buffer) {
		printk(KERN_WARNING "input buffer is null\n");
		ret = -EFAULT;
		goto proc_read_out;

	} else if (offset == 0) {
		SEC_BP_TRACE("offset == 0: ~~~~~~~~\n");

		/* we need to reset here, e.g. if there is any previous
		 * incomplete read */
		bp_stats_proc_read_ctx.next_expected_seq_offset = 0;
		bp_stats_proc_read_ctx.is_no_more_data = 0;
		bp_stats_proc_read_ctx.who = SEC_BP_FILL_HEADER; /* start */
		bp_stats_proc_read_ctx.next_idx = 0;
		bp_stats_proc_read_ctx.next_sub_idx = 0;
		bp_stats_proc_read_ctx.next_idx_is_done = 0;
		bp_stats_proc_read_ctx.fname_is_done = 0;

	} else if (bp_stats_proc_read_ctx.is_no_more_data) {
		SEC_BP_TRACE("is_eof: ~~~~~~~~~~~~~~\n");
		*eof = 1;
		ret = 0;
		goto proc_read_out;

	} else if (offset != bp_stats_proc_read_ctx.next_expected_seq_offset) {
		printk(KERN_WARNING "read in random order, not supported, "
				"offset = %ld vs %ld\n",
				bp_stats_proc_read_ctx.next_expected_seq_offset, offset);
		*eof = 1;
		ret = -ESPIPE;
		goto proc_read_out;
	}

	len_copied = sec_bp_proc_fill_report(buffer, offset, buffer_length);
	ret = len_copied;
	SEC_BP_TRACE("len_copied= %d\n", len_copied);

proc_read_out:

	return ret;
}

static int sec_bp_proc_fill_report(char *buffer, off_t offset,
		int buffer_length)
{
	int filled_len = 0;

	SEC_BP_TRACE("offset= %ld, buffer_length= %d\n",
			(long)offset, buffer_length);

	if (!bp_stats_proc_read_ctx.line_cache) {

		SEC_BP_TRACE("alloca line cache\n");
		bp_stats_proc_read_ctx.line_cache =
			(char *) vmalloc(SEC_BP_LINE_CACHE_SIZE);
		if (!bp_stats_proc_read_ctx.line_cache) {
			printk(KERN_WARNING "out of memory\n");
			return -ENOMEM;
		}
	}

	BUG_ON(bp_stats_proc_read_ctx.is_no_more_data);

	/* header */
	if (bp_stats_proc_read_ctx.who == SEC_BP_FILL_HEADER) {
		SEC_BP_TRACE("who == %d\n", bp_stats_proc_read_ctx.who);
		filled_len += sec_bp_proc_fill_header(buffer, buffer_length);
	}

	/* proc_stat */
	if (bp_stats_proc_read_ctx.who == SEC_BP_FILL_CPUSTAT
			&& filled_len < buffer_length) {
		SEC_BP_TRACE("who == %d\n", bp_stats_proc_read_ctx.who);
		filled_len += sec_bp_proc_fill_cpu_stat(buffer + filled_len,
				buffer_length - filled_len);
	}

	/* proc_ps_stat */
	if (bp_stats_proc_read_ctx.who == SEC_BP_FILL_PROC_PS
			&& filled_len < buffer_length) {
		SEC_BP_TRACE("who == %d\n", bp_stats_proc_read_ctx.who);
		filled_len += sec_bp_proc_fill_process_stat(buffer + filled_len,
				buffer_length - filled_len);
	}

	/* proc_diskstats */
	if (bp_stats_proc_read_ctx.who == SEC_BP_FILL_DISKSTATS
			&& filled_len < buffer_length) {
		SEC_BP_TRACE("who == %d\n", bp_stats_proc_read_ctx.who);
		filled_len += sec_bp_proc_fill_diskstats(buffer + filled_len,
				buffer_length - filled_len);
	}

	/* last step */
	if (bp_stats_proc_read_ctx.who == SEC_BP_FILL_DONE) {
		SEC_BP_TRACE("who == %d\n", bp_stats_proc_read_ctx.who);
		bp_stats_proc_read_ctx.is_no_more_data = 1;
		vfree(bp_stats_proc_read_ctx.line_cache);
		bp_stats_proc_read_ctx.line_cache = NULL;
	} else {
		BUG_ON(bp_stats_proc_read_ctx.who < SEC_BP_FILL_HEADER
				|| bp_stats_proc_read_ctx.who >= SEC_BP_FILL_DONE);
	}

	SEC_BP_TRACE("filled_len= %d\n", filled_len);
	BUG_ON(filled_len > buffer_length);
	return filled_len;
}

static int sec_bp_proc_fill_header(char *buffer, int buffer_length)
{
	int filled_len = 0;
	int bs_header_len = 0;
	BUG_ON(bp_stats_proc_read_ctx.who != SEC_BP_FILL_HEADER);
	BUG_ON(!bs_header);

	if (!bs_header) {  /* silently skip header */
		bp_stats_proc_read_ctx.next_idx = 0;
		++bp_stats_proc_read_ctx.who;
		return 0;
	}

	bs_header_len = strlen(bs_header);
	if (bp_stats_proc_read_ctx.next_idx < bs_header_len) {
		filled_len = (bs_header_len
				- bp_stats_proc_read_ctx.next_idx); /* remaining len */
		filled_len = min(buffer_length, filled_len); /* adjusted len */
		memcpy(buffer, bs_header + bp_stats_proc_read_ctx.next_idx,
				filled_len);
		bp_stats_proc_read_ctx.next_idx += filled_len;
	} else {
		bp_stats_proc_read_ctx.next_idx = 0;
		++bp_stats_proc_read_ctx.who;
	}

	bp_stats_proc_read_ctx.next_expected_seq_offset += filled_len;
	bp_stats_proc_read_ctx.is_no_more_data = 0;

	return filled_len;
}

static int sec_bp_proc_fill_cpu_stat(char *buffer, int buffer_length)
{
	int ii = 0;
	int line_len = 0, length = 0;
	int len_copied = 0;
	int next_idx_done_cnt = 2;
	unsigned int i;

	SEC_BP_TRACE("buffer_length= %d\n", buffer_length);

	SEC_BP_TRACE("next_idx= %d, next_sub_idx= %d, next_idx_done_cnt= %d\n",
			bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx,
			bp_stats_proc_read_ctx.next_idx_is_done);

	if (!bp_stats_proc_read_ctx.fname_is_done) {

		BUG_ON(bp_stats_proc_read_ctx.next_idx
				|| bp_stats_proc_read_ctx.next_idx_is_done);

		line_len = sizeof("#@" PROC_STAT_LOG "\n") - 1;
		if (line_len > buffer_length) {
			SEC_BP_TRACE("#@%s: line_len= %d > %d: -- 00 --\n",
					PROC_STAT_LOG, line_len, buffer_length);
			return 0;
		}

		memcpy(buffer, "#@" PROC_STAT_LOG "\n", line_len);
		SEC_BP_TRACE("ii= %d: [0  => %d]: %d\n",
				ii, line_len, buffer_length);
		len_copied = line_len;
		next_idx_done_cnt = 0;
		bp_stats_proc_read_ctx.fname_is_done = 1;
	}

	for (ii = bp_stats_proc_read_ctx.next_idx; ii < sec_cs_idx; ++ii) {

		next_idx_done_cnt = 0;

		if (ii != bp_stats_proc_read_ctx.next_idx
				|| !bp_stats_proc_read_ctx.next_idx_is_done) {

			SEC_BP_TRACE("first idx not not complete: idx= %d\n",
					bp_stats_proc_read_ctx.next_idx);

			line_len = snprintf(bp_stats_proc_read_ctx.line_cache, SEC_BP_LINE_CACHE_SIZE, "%lu\n",
					sec_cs_table[0][ii].uptime);

			if (len_copied + line_len > buffer_length) {
				SEC_BP_TRACE("len_copied + line_len = %d + %d > %d: -- 0 --\n",
						len_copied, line_len, buffer_length);
				break;
			}

			memcpy(buffer + len_copied, bp_stats_proc_read_ctx.line_cache,
					line_len);
			SEC_BP_TRACE("ii= %d: [%d + %d => %d]: %d\n",
					ii, len_copied, line_len,
					len_copied + line_len, buffer_length);
			len_copied += line_len;
		}

		next_idx_done_cnt = 1;

		SEC_BP_TRACE("ii= %d: fill_stats\n", ii);
		length = 0;
		for_each_online_cpu(i)
		{
			length += snprintf(bp_stats_proc_read_ctx.line_cache+length,
					SEC_BP_LINE_CACHE_SIZE-length,
					"cpu%u  %llu %llu %llu %llu %llu"
					" %llu %llu %llu %llu %llu\n", i,
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].user),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].nice),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].system),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].idle),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].iowait),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].irq),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].softirq),
					(unsigned long long)
					cputime64_to_clock_t(sec_cs_table[i][ii].steal),
					(unsigned long long)
						cputime64_to_clock_t(sec_cs_table[i][ii].user_rt),
					(unsigned long long)
						cputime64_to_clock_t(sec_cs_table[i][ii].system_rt));
		}
		length += snprintf(bp_stats_proc_read_ctx.line_cache + length, SEC_BP_LINE_CACHE_SIZE-length, "\n");

		line_len = length;
		if (len_copied + line_len > buffer_length) {
			break;
		}

		memcpy(buffer + len_copied, bp_stats_proc_read_ctx.line_cache,
				line_len);
		SEC_BP_TRACE("ii= %d:  [%d + %d => %d]: %d\n",
				ii, len_copied, line_len,
				len_copied + line_len, buffer_length);
		len_copied += line_len;

		next_idx_done_cnt = 2;
	}

	bp_stats_proc_read_ctx.next_expected_seq_offset += len_copied;
	bp_stats_proc_read_ctx.is_no_more_data = 0;


	SEC_BP_TRACE("ii= %d: next_idx= %d, "
			"next_sub_idx= %d, next_idx_done_cnt= %d: -- Pre ---\n",
			ii, bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx, next_idx_done_cnt);

	BUG_ON(!bp_stats_proc_read_ctx.fname_is_done);
	if (ii == sec_cs_idx) {
		SEC_BP_TRACE("ii= %d: Data finished\n", ii);
		++bp_stats_proc_read_ctx.who;
		bp_stats_proc_read_ctx.next_idx = 0;
		bp_stats_proc_read_ctx.next_sub_idx = 0;
		BUG_ON(next_idx_done_cnt != 2);
		bp_stats_proc_read_ctx.next_idx_is_done = 0;
		/* bp_stats_proc_read_ctx.is_no_more_data = 1; */
		bp_stats_proc_read_ctx.fname_is_done = 0;
	} else {
		SEC_BP_TRACE("ii= %d: more_data\n", ii);
		BUG_ON(!(next_idx_done_cnt == 0 || next_idx_done_cnt == 1));
		BUG_ON(bp_stats_proc_read_ctx.who != SEC_BP_FILL_CPUSTAT);
		bp_stats_proc_read_ctx.next_idx = ii;
		bp_stats_proc_read_ctx.next_sub_idx = 0; /* not used */
		bp_stats_proc_read_ctx.next_idx_is_done = (next_idx_done_cnt != 0);
	}

	SEC_BP_TRACE("ii= %d: next_idx= %d, "
			"next_sub_idx= %d, next_idx_done_cnt= %d: -- Post ---\n",
			ii, bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx,
			bp_stats_proc_read_ctx.next_idx_done_cnt);

	BUG_ON(len_copied > buffer_length);
	return len_copied;
}

static int sec_bp_proc_fill_process_stat(char *buffer, int buffer_length)
{
	int ii = 0;
	int jj = 0;
	int end_idx = 0;
	int line_len = 0;
	int len_copied = 0;
	int next_idx_is_done = 0;
	int is_loop_break = 0;

	SEC_BP_TRACE("buffer_length= %d\n", buffer_length);

	SEC_BP_TRACE("next_idx= %d, next_sub_idx= %d, next_idx_is_done= %d\n",
			bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx,
			bp_stats_proc_read_ctx.next_idx_is_done);

	if (!bp_stats_proc_read_ctx.fname_is_done) {

		BUG_ON(bp_stats_proc_read_ctx.next_idx
				|| bp_stats_proc_read_ctx.next_idx_is_done);

		line_len = sizeof("#@" PROC_PS_LOG "\n") - 1;
		if (line_len > buffer_length) {
			SEC_BP_TRACE("#@%s: line_len= %d > %d: -- 00 --\n",
					PROC_PS_LOG, line_len, buffer_length);
			return 0;
		}

		memcpy(buffer, "#@" PROC_PS_LOG "\n", line_len);
		SEC_BP_TRACE("ii= %d: [0  => %d]: %d\n",
				ii, line_len, buffer_length);
		len_copied = line_len;
		bp_stats_proc_read_ctx.fname_is_done = 1;
	}

	for (ii = bp_stats_proc_read_ctx.next_idx;
			!is_loop_break && ii < sec_ps_ut_idx; ++ii) {

		if (ii != bp_stats_proc_read_ctx.next_idx
				|| !bp_stats_proc_read_ctx.next_idx_is_done) {

			SEC_BP_TRACE("first idx not not complete: idx= %d\n",
					bp_stats_proc_read_ctx.next_idx);

			line_len = snprintf(bp_stats_proc_read_ctx.line_cache, SEC_BP_LINE_CACHE_SIZE, "%lu\n",
					sec_ps_ut_table[ii].uptime);

			if (len_copied + line_len > buffer_length) {
				SEC_BP_TRACE("len_copied + line_len = %d + %d > %d: -- 0 --\n",
						len_copied, line_len, buffer_length);
				next_idx_is_done = 0;
				break;
			}

			memcpy(buffer + len_copied, bp_stats_proc_read_ctx.line_cache,
					line_len);
			SEC_BP_TRACE("ii= %d: [%d + %d => %d]: %d\n",
					ii, len_copied, line_len,
					len_copied + line_len, buffer_length);
			len_copied += line_len;

			jj = sec_ps_ut_table[ii].start_idx;
		} else {
			jj = bp_stats_proc_read_ctx.next_sub_idx;
		}
		SEC_BP_TRACE("ii= %d: first jj= %d\n", ii, jj);

		next_idx_is_done = 1;
		if (ii == sec_ps_ut_idx - 1)
			end_idx = sec_ps_idx;
		else
			end_idx = sec_ps_ut_table[ii+1].start_idx;

		for (; jj < end_idx; ++jj) {
			line_len = snprintf(bp_stats_proc_read_ctx.line_cache,
					SEC_BP_LINE_CACHE_SIZE,
					SEC_BP_PROC_PS_FMT_STR,
					sec_ps_table[jj].pid,
					sec_ps_table[jj].tcomm,
#if (SEC_BP_COMM_WITH_PID != 0)
					sec_ps_table[jj].pid, /* append to tcomm */
#endif
					sec_ps_table[jj].state,
					sec_ps_table[jj].ppid,
					cputime_to_clock_t(sec_ps_table[jj].utime),
					cputime_to_clock_t(sec_ps_table[jj].stime),
					(unsigned long long)sec_ps_table[jj].start_time,
					(unsigned int)sec_ps_table[jj].cpuid /* cpuid */
					);

			/* lalit: added +1 because we have a terminating new line
			   at end of the block (see below). */
			if (len_copied + line_len + 1 > buffer_length) {
				is_loop_break = 1;
				break;
			}

			memcpy(buffer + len_copied, bp_stats_proc_read_ctx.line_cache, line_len);
			SEC_BP_TRACE("ii= %d, jj= %d: [%d + %d => %d]: %d\n",
					ii, jj, len_copied, line_len,
					len_copied + line_len, buffer_length);
			len_copied += line_len;
		}

		/* Do not add new line if upper loop finished pre-maturely (we
		 * had to break because of buffer full, net read call will
		 * resume again */
		if (!is_loop_break) {
			SEC_BP_TRACE("ii= %d: [%d + NL => %d]: %d\n",
					ii, len_copied,
					len_copied + 1, buffer_length);
			BUG_ON(len_copied >= buffer_length);
			buffer[len_copied++] = '\n';
		}
	}

	bp_stats_proc_read_ctx.next_expected_seq_offset += len_copied;
	bp_stats_proc_read_ctx.is_no_more_data = 0;


	SEC_BP_TRACE("ii= %d, jj= %d: next_idx= %d, "
			"next_sub_idx= %d, next_idx_is_done= %d: -- Pre ---\n",
			ii, jj, bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx, next_idx_is_done);

	BUG_ON(!bp_stats_proc_read_ctx.fname_is_done);
	if (ii == sec_ps_ut_idx && jj == end_idx) {
		SEC_BP_TRACE("ii= %d, jj= %d: Data finished\n", ii, jj);
		++bp_stats_proc_read_ctx.who;
		bp_stats_proc_read_ctx.next_idx = 0;
		bp_stats_proc_read_ctx.next_sub_idx = 0;
		BUG_ON(is_loop_break);
		bp_stats_proc_read_ctx.next_idx_is_done = 0;
		/* bp_stats_proc_read_ctx.is_no_more_data = 1; */
		bp_stats_proc_read_ctx.fname_is_done = 0;
	} else {
		SEC_BP_TRACE("ii= %d, jj= %d: more_data\n", ii, jj);
		BUG_ON(bp_stats_proc_read_ctx.who != SEC_BP_FILL_PROC_PS);
		bp_stats_proc_read_ctx.next_idx = is_loop_break ? ii - 1 : ii;
		bp_stats_proc_read_ctx.next_sub_idx = jj;
		bp_stats_proc_read_ctx.next_idx_is_done = next_idx_is_done;
	}

	SEC_BP_TRACE("ii= %d, jj= %d: next_idx= %d, "
			"next_sub_idx= %d, next_idx_is_done= %d: -- Post ---\n",
			ii, jj, bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx,
			bp_stats_proc_read_ctx.next_idx_is_done);

	BUG_ON(len_copied > buffer_length);
	return len_copied;
}

static int sec_bp_proc_fill_diskstats(char *buffer, int buffer_length)
{
	int ii = 0;
	int jj = 0;
	int end_idx = 0;
	int line_len = 0;
	int len_copied = 0;
	int next_idx_is_done = 0;
	int is_loop_break = 0;

	SEC_BP_TRACE("buffer_length= %d\n", buffer_length);

	SEC_BP_TRACE("next_idx= %d, next_sub_idx= %d, next_idx_is_done= %d\n",
			bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx,
			bp_stats_proc_read_ctx.next_idx_is_done);

	if (!bp_stats_proc_read_ctx.fname_is_done) {

		BUG_ON(bp_stats_proc_read_ctx.next_idx
				|| bp_stats_proc_read_ctx.next_idx_is_done);

		line_len = sizeof("#@" PROC_DISKSTATS_LOG "\n") - 1;
		if (line_len > buffer_length) {
			SEC_BP_TRACE("#@%s: line_len = %d  > %d: -- 00 --\n",
					PROC_DISKSTATS_LOG, line_len, buffer_length);
			return 0;
		}

		memcpy(buffer, "#@" PROC_DISKSTATS_LOG "\n", line_len);
		SEC_BP_TRACE("ii= %d: [0  => %d]: %d\n",
				ii, line_len, buffer_length);
		len_copied = line_len;
		bp_stats_proc_read_ctx.fname_is_done = 1;
	}

	for (ii = bp_stats_proc_read_ctx.next_idx;
			!is_loop_break && ii < sec_ds_ut_idx; ++ii) {

		if (ii != bp_stats_proc_read_ctx.next_idx
				|| !bp_stats_proc_read_ctx.next_idx_is_done) {

			SEC_BP_TRACE("first idx not not complete: idx= %d\n",
					bp_stats_proc_read_ctx.next_idx);

			line_len = snprintf(bp_stats_proc_read_ctx.line_cache, SEC_BP_LINE_CACHE_SIZE, "%lu\n",
					sec_ds_ut_table[ii].uptime);

			if (len_copied + line_len > buffer_length) {
				SEC_BP_TRACE("len_copied + line_len = %d + %d > %d: -- 0 --\n",
						len_copied, line_len, buffer_length);
				next_idx_is_done = 0;
				break;
			}

			memcpy(buffer + len_copied, bp_stats_proc_read_ctx.line_cache,
					line_len);
			SEC_BP_TRACE("ii= %d: [%d + %d => %d]: %d\n",
					ii, len_copied, line_len,
					len_copied + line_len, buffer_length);
			len_copied += line_len;

			jj = sec_ds_ut_table[ii].start_idx;
		} else {
			jj = bp_stats_proc_read_ctx.next_sub_idx;
		}
		SEC_BP_TRACE("ii= %d: first jj= %d\n", ii, jj);

		next_idx_is_done = 1;
		if (ii == sec_ds_ut_idx - 1)
			end_idx = sec_ds_idx;
		else
			end_idx = sec_ds_ut_table[ii+1].start_idx;

		for (; jj < end_idx; ++jj) {
			/* major first_minor name ios[0] merges[0] nsect_read
			   ticks[0] ios[1] merges[1] nsect_read sect_write
			   ticks[1] in_flight io_ticks time_in_queue */
			/* "%4d %4d %s %lu %lu %llu %u %lu %lu %llu %u %u %u %u\n" */
			line_len = snprintf(bp_stats_proc_read_ctx.line_cache,
					SEC_BP_LINE_CACHE_SIZE,
					"0 0 %s 0 0 %llu 0 0 0 %llu 0 0 %u 0\n",
#if (SEC_DS_USE_SUMMARY == 0)
					sec_ds_table[jj].name,
#else
					"sum_bdev",
#endif
					(unsigned long long)
					sec_ds_table[jj].nsect_read,
					(unsigned long long)
					sec_ds_table[jj].nsect_write,
					jiffies_to_msecs(sec_ds_table[jj].io_ticks));

			/* lalit: added +1 because we have a terminating new line
			   at end of the block (see below). */
			if (len_copied + line_len + 1 > buffer_length) {
				is_loop_break = 1;
				break;
			}

			memcpy(buffer + len_copied, bp_stats_proc_read_ctx.line_cache, line_len);
			SEC_BP_TRACE("ii= %d, jj= %d: [%d + %d => %d]: %d\n",
					ii, jj, len_copied, line_len,
					len_copied + line_len, buffer_length);
			len_copied += line_len;
		}

		/* Do not add new line if upper loop finished pre-maturely (we
		 * had to break because of buffer full, net read call will
		 * resume again */
		if (!is_loop_break) {
			SEC_BP_TRACE("ii= %d: [%d + NL => %d]: %d\n",
					ii, len_copied,
					len_copied + 1, buffer_length);
			BUG_ON(len_copied >= buffer_length);
			buffer[len_copied++] = '\n';
		}
	}

	bp_stats_proc_read_ctx.next_expected_seq_offset += len_copied;
	bp_stats_proc_read_ctx.is_no_more_data = 0;

	SEC_BP_TRACE("ii= %d, jj= %d: next_idx= %d, "
			"next_sub_idx= %d, next_idx_is_done= %d: -- Pre ---\n",
			ii, jj, bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx, next_idx_is_done);

	BUG_ON(!bp_stats_proc_read_ctx.fname_is_done);
	if (ii == sec_ds_ut_idx && jj == end_idx) {
		SEC_BP_TRACE("ii= %d, jj= %d: Data finished\n", ii, jj);
		++bp_stats_proc_read_ctx.who;
		bp_stats_proc_read_ctx.next_idx = 0;
		bp_stats_proc_read_ctx.next_sub_idx = 0;
		BUG_ON(is_loop_break);
		bp_stats_proc_read_ctx.next_idx_is_done = 0;
		/* bp_stats_proc_read_ctx.is_no_more_data = 1; */
		bp_stats_proc_read_ctx.fname_is_done = 0;
	} else {
		SEC_BP_TRACE("ii= %d, jj= %d: more_data\n", ii, jj);
		BUG_ON(bp_stats_proc_read_ctx.who != SEC_BP_FILL_DISKSTATS);
		bp_stats_proc_read_ctx.next_idx = is_loop_break ? ii - 1 : ii;
		bp_stats_proc_read_ctx.next_sub_idx = jj;
		bp_stats_proc_read_ctx.next_idx_is_done = next_idx_is_done;
	}

	SEC_BP_TRACE("ii= %d, jj= %d: next_idx= %d, "
			"next_sub_idx= %d, next_idx_is_done= %d: -- Post ---\n",
			ii, jj, bp_stats_proc_read_ctx.next_idx,
			bp_stats_proc_read_ctx.next_sub_idx,
			bp_stats_proc_read_ctx.next_idx_is_done);

	BUG_ON(len_copied > buffer_length);
	return len_copied;
}

static inline int sec_bp_proc_check_length(int buffer_length)
{
	return buffer_length > SEC_BP_PROC_MIN_READ_LEN;
}


/**********************************************************************/

static void sec_bp_print_help(void)
{
	printk("Boot Profile Help:\n"
			"==========================================\n\n"
			"proc_read:    cat /proc/bootstats\n\n"
			"help:         echo h > /proc/bootstats\n"
			"start:        echo 1 > /proc/bootstats\n"
			"direct-dump:  echo d > /proc/bootstats\n"
			"clear memory: echo c > /proc/bootstats\n"
			"status print: echo s > /proc/bootstats\n\n");
}

static void sec_bp_print_status(void)
{

	if (sec_bp_state == SEC_BP_STATE_COMPLETING) {
		BUG_ON(!sec_cs_table[0]);
		printk("Boot Profile Status: COMPLETING\n");
	} else if (sec_bp_state == SEC_BP_STATE_COMPLETED) {
		BUG_ON(!sec_cs_table[0]);
		printk("Boot Profile Status: COMPLETED\n");
		sec_bp_meminfo_dump();
	} else if (sec_bp_state == SEC_BP_STATE_UNINITED) {
		printk("Boot Profile Status: NOT STARTED - MEMORY CLEARED\n");
	} else {
		BUG_ON(sec_bp_state != SEC_BP_STATE_RUNNING
				&& sec_bp_state != SEC_BP_STATE_STARTING);
		printk("Boot Profile Status: RUNNING\n");
	}
}

static inline void sec_bp_start(void)
{
	SEC_BP_TRACE("enter\n");
	if (sec_bp_state == SEC_BP_STATE_UNINITED
			|| sec_bp_state == SEC_BP_STATE_COMPLETED) {
		SEC_BP_TRACE("starting\n");
		sec_bp_start__();
	} else {
		BUG_ON(sec_bp_state != SEC_BP_STATE_RUNNING
				&& sec_bp_state != SEC_BP_STATE_STARTING);
		printk(KERN_INFO "already running\n");
	}
}

/**********************************************************************/
#if (SEC_BP_WAIT_UPTIME_SEC != 0)
void sec_uptime_wait(int nsec)
{
	int sleep_delay = 0;
	struct timespec uptime;
	do_posix_clock_monotonic_gettime(&uptime);
	sleep_delay = nsec - uptime.tv_sec;

	if (sleep_delay > 0) {
		printk("%s: sleeping for %d ms\n", __FUNCTION__, sleep_delay);
		msleep(sleep_delay * 1000);
	}

	do_posix_clock_monotonic_gettime(&uptime);
	printk("%s: Uptime: %u.%u\n", __FUNCTION__,
			(unsigned)uptime.tv_sec, (unsigned)uptime.tv_nsec);
}
#endif

/*
 * find the usb connect from "/dev/usb/usblog" file. first its open the file
 * and find the MountDit. This code work only the case when exeDSP running.
 */
static int find_usb_device(char *dev, int max_path_len)
{
	struct file *fp;
	char buf[256];
	char *p;
	int ret = -ENOENT;
	char *dup_str = NULL,  *alt_dup_str = NULL;
	mm_segment_t oldfs = get_fs();
	char *tok = NULL;

	fp = filp_open("/dtv/usb/usblog", O_RDONLY | O_LARGEFILE, 0600);

	if (IS_ERR(fp)) {
		printk("open error \n");
		return 1;
	}

	fp->f_pos = 0;
	set_fs(KERNEL_DS);
	ret = fp->f_op->read(fp, buf, sizeof(buf)-1, &fp->f_pos);

	if (ret <= 0) {
		printk("read error\n");
		filp_close(fp, NULL);
		set_fs(oldfs);
		return 1;
	}

	filp_close(fp, NULL);
	BUG_ON(ret > sizeof(buf)-1);
	buf[ret] = '\0';

	p = strstr(buf, "MountDir");

	if (p) {
		/* sscanf(p, "MountDir : %s", dev);*/  /* Prevent error */
		/*
		 * duplicating the string to be parsed also kstrdup
		 * allocate memory iteslf
		 */
		dup_str = kstrdup(p, GFP_KERNEL);
		if (!dup_str)
			return 1;

		alt_dup_str = dup_str;

		/*
		 * tokenising the duplicated string to find out the Mount dir
		 * value for this string "mount_dir : /dev/sda1")
		 */
		tok = strsep(&alt_dup_str, ":");
		if (!tok)
			return 1;

		/*
		 * truncating the string to end of line "\n" delimiter and get
		 * the mount dir i.e /dev/sda1 "
		 */
		tok = strsep(&alt_dup_str, "\n");
		if (!tok)
			return 1;

		tok = strim(tok); /*  trimming the white space characters in token */
		if (!tok)
			return 1;

		strncpy(dev, tok, max_path_len);
		dev[max_path_len - 1] = '\0'; /* for boundary cond. */

		kfree(dup_str);

		set_fs(oldfs);
		return 0;
	}
	return 1; /* no usb device */
}

/*
 * Its a general solution to get the mounted usb device path if exeDSP
 * is not running.
 */
int usb_detect(char *usb_path, int max_path_len)
{
	unsigned char *read_buf = NULL;
	struct file *filp = NULL;
	int ret = 0;
	int bytesread = 0;
	char *ptr = NULL, *ptr2 = NULL; /* for parsing string */
	int len = 0;
	const int READ_BUF_SIZE = 4096;
	mm_segment_t oldfs = get_fs();

	if (!usb_path || max_path_len < 1) {
		printk("BootChart: Error Invalid arguments\n");
		ret = -1;
		goto out;
	}
	usb_path[0] = '\0'; /* in case path is not found */

	/* allocate sufficient size to read mount entries */
	read_buf = (char *)vmalloc(READ_BUF_SIZE);
	if (!read_buf) {
		printk("BootChart: read_buf: no memory\n");
		ret = -1;
		goto out; /* no memory!! */
	}

	set_fs(KERNEL_DS);
	/* opening file to know where usb has mounted */
	filp = filp_open("/proc/mounts", O_RDONLY | O_LARGEFILE, 0);
	if (IS_ERR(filp) || (filp == NULL)) {
		printk("BootChart: error opening /proc/mounts\n");
		ret = -1;
		goto out;
	}

	if (filp->f_op->read == NULL) {
		printk("BootChart: read not allowed\n");
		ret = -1;
		goto out_close;
	}

	filp->f_pos = 0;

	printk("BootChart: USB MOUNT PATH(s):\n");

	/* reading a /proc/mount file here (to know where usb has mounted)
	 * read max 4 KB buffer of file, that should be sufficient.
	 */

	bytesread = filp->f_op->read(filp, read_buf, READ_BUF_SIZE, &filp->f_pos);
	if (bytesread <= 0) {
		printk("BootChart: error reading file\n");
		ret = -1;
		goto out_close;
	}
	read_buf[bytesread-1] = '\0';

	filp_close(filp, NULL);

	/*
	 * The idea for this loop is to take all the string
	 * started  with  "/dev/sd"  (usb  make dev node in
	 * /dev/sd** )
	 * the format of that file is
	 * "devnode"  "moount dir" "option" ...............
	 * by this way we can find the directory where usb
	 * has mounted
	 */
	ptr = strstr(read_buf, "/dev/sd");
	if (!ptr) {
		printk("BootChart: USB not connected\n");
		ret = -1;
		goto out;
	}

	ptr = strstr(ptr + 1, " ");
	if (!ptr) {
		printk("BootChart: Error USB path detection [1]\n");
		ret = -1;
		goto out;
	}

	ptr = strchr(ptr + 1, '/');
	if (!ptr) {
		printk("BootChart: Error USB path detection [2]\n");
		ret = -1;
		goto out;
	}

	ptr2 = strchr(ptr + 1, ' ');
	if (!ptr2) {
		printk("BootChart: Error USB path detection [3]\n");
		ret = -1;
		goto out;
	}

	BUG_ON(ptr2 - ptr <= 0);
	len = min(ptr2 - ptr + 1, max_path_len);
	memcpy(usb_path, ptr, len);
	usb_path[len - 1] = '\0';
	printk("\t %s \n", usb_path);

out_close:
	filp_close(filp, NULL);

out:
	set_fs(oldfs);
	if (read_buf)
		vfree(read_buf);

	return ret;
}

/*
 * This function called to save the bootchart log into the
 * usb device connected with target. First its try to find
 * usb device in case
 */
static int save_to_usb()
{
	struct file *fp = NULL;
	char *line = NULL;
	const int USB_BUFF_LEN = 4096;
	off_t offset = 0;
	char path[120];
	int ret = 0;
	int line_copied = 0;
	char dev[512];
	mm_segment_t oldfs = get_fs();

	if (find_usb_device(dev, sizeof(dev)) != 0)	{
		if (usb_detect(dev, sizeof(dev)) != 0) {
			printk(KERN_DEBUG "Usb open fail\n");
			return 0;
		}
	}

	snprintf(path, sizeof(path), "%s/%s", dev, "bootstats");
	line = (char *) vmalloc(USB_BUFF_LEN);
	if (!line) {
		printk("[%s: %d]line: mem alloc failure\n", __FUNCTION__, __LINE__);
		return 0;
	}

	printk("open file %s ---- \n", path);

	set_fs(KERNEL_DS);
	fp = filp_open(path, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0600);
	if (IS_ERR(fp)) {
		printk("Failed to open file %s ---- exiting\n", path);
		if (line) {
			vfree(line);
			line = NULL;
		}
		set_fs(oldfs);
		return 0;
	}

	while (!bp_stats_proc_read_ctx.is_no_more_data) {
		line_copied = sec_bp_proc_fill_report(line, offset, USB_BUFF_LEN);

		ret = fp->f_op->write(fp, line, line_copied, &fp->f_pos);

		if (ret < 0) {
			printk(KERN_DEBUG "Write proc_stat.log is fail\n");
			if (line) {
				vfree(line);
				line = NULL;
			}
			filp_close(fp, NULL);
			set_fs(oldfs);
			return 0;
		}
	}

	if (line) {
		vfree(line);
		line = NULL;
	}
	filp_close(fp, NULL);
	set_fs(oldfs);
	return 1;
}
#endif/* Boot Chart */
