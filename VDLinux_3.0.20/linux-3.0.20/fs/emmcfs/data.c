/**
 * @file	fs/emmcfs/data.c
 * @brief	Basic data operations.
 * @author	Ivan Arishchenko, i.arishchenk@samsung.com
 * @date	05/05/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file implements bio data operations and its related functions.
 *
 * @see		TODO: documents
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/buffer_head.h>
#include <linux/mpage.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/pagemap.h>

#include "emmcfs.h"
#include "debug.h"


static void snapshot_finalize_bio(struct bio *bio)
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
	struct bio_vec *bvec = bio->bi_io_vec + bio->bi_vcnt - 1;

	do {
		struct page *page = bvec->bv_page;

		if (--bvec >= bio->bi_io_vec)
			prefetchw(&bvec->bv_page->flags);
		if (!uptodate) {
			SetPageError(page);
			if (page->mapping)
				set_bit(AS_EIO, &page->mapping->flags);
			EMMCFS_BUG();
		}
		end_page_writeback(page);
		unlock_page(page);
		__free_pages(page, 0);

	} while (bvec >= bio->bi_io_vec);
}

/**
 * @brief 		Finalize snapshot IO writing.
 * param [in]	bio	BIO structure to be written.
 * param [in]	err	With error or not.
 * @return	void
 */
static void snapshot_end_head_write(struct bio *bio, int err)
{
	struct emmcfs_sb_info *sbi = bio->bi_private;

	snapshot_finalize_bio(bio);

	EMMCFS_BUG_ON(!sbi);

	/* Unlock write semaphore */
	up_write(&sbi->snapshot_info->head_write_sem);

	bio_put(bio);
}

/**
 * @brief 		Finalize snapshot IO writing.
 * param [in]	bio	BIO structure to be written.
 * param [in]	err	With error or not.
 * @return	void
 */
static void snapshot_end_body_write(struct bio *bio, int err)
{
	struct emmcfs_sb_info *sbi = bio->bi_private;

	snapshot_finalize_bio(bio);

	EMMCFS_BUG_ON(!sbi);

	/* Unlock read semaphore */
	up_read(&sbi->snapshot_info->head_write_sem);

	bio_put(bio);
}

/**
 * @brief 		Finalize IO writing.
 * param [in] 	bio	BIO structure to be written.
 * param [in] 	err	With error or not.
 * @return	void
 */
static void end_io_write(struct bio *bio, int err)
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
	struct bio_vec *bvec = bio->bi_io_vec + bio->bi_vcnt - 1;

	do {
		struct page *page = bvec->bv_page;

		if (--bvec >= bio->bi_io_vec)
			prefetchw(&bvec->bv_page->flags);
		if (!uptodate) {
			SetPageError(page);
			if (page->mapping)
				set_bit(AS_EIO, &page->mapping->flags);
			EMMCFS_BUG();
		}
		end_page_writeback(page);
	} while (bvec >= bio->bi_io_vec);

	if (bio->bi_private)
		complete(bio->bi_private);
	bio_put(bio);
}

/**
 * @brief 		Finalize IO writing.
 * param [in] 	bio	BIO structure to be read.
 * param [in] 	err	With error or not.
 * @return	void
 */
static void read_end_io(struct bio *bio, int err)
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
	struct bio_vec *bvec = bio->bi_io_vec + bio->bi_vcnt - 1;
	struct completion *wait = bio->bi_private;

	do {
		struct page *page = bvec->bv_page;

		if (--bvec >= bio->bi_io_vec)
			prefetchw(&bvec->bv_page->flags);

		if (uptodate) {
			SetPageUptodate(page);
		} else {
			ClearPageUptodate(page);
			SetPageError(page);
		}
	} while (bvec >= bio->bi_io_vec);
	complete(wait);
	bio_put(bio);
}

/**
 * @brief 			Allocate new BIO.
 * @param [in] 	bdev		The eMMCFS superblock information.
 * @param [in] 	first_sector	BIO first sector.
 * @param [in] 	nr_vecs		Number of BIO pages.
 * @return 			Returns pointer to allocated BIO structure.
 */
struct bio *allocate_new_bio(struct block_device *bdev, sector_t first_sector,
		int nr_vecs)
{
	/*int nr_vecs;*/
	gfp_t gfp_flags = GFP_NOFS | __GFP_HIGH;
	struct bio *bio = NULL;
	sector_t s_count = bdev->bd_inode->i_size >> SECTOR_SIZE_SHIFT;

	if ((first_sector > s_count) || ((first_sector + nr_vecs) > s_count))
		return ERR_PTR(-EFAULT);

	bio = bio_alloc(gfp_flags, nr_vecs);

	if (bio == NULL && (current->flags & PF_MEMALLOC)) {
		while (!bio && (nr_vecs /= 2))
			bio = bio_alloc(gfp_flags, nr_vecs);
	}

	if (bio) {
		bio->bi_bdev = bdev;
		bio->bi_sector = first_sector;
	}

	return bio;
}


#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	#define SUBMIT_BIO_WAIT_FOR_FLUSH_FLAGS WRITE_BARRIER
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	#define SUBMIT_BIO_WAIT_FOR_FLUSH_FLAGS WRITE_FLUSH_FUA
#else
	BUILD_BUG();
#endif

/**
 * @brief 			Write page to the given sector address.
 *        			Write the locked page to the sector address.
 *        			Write operation is synchronous, and caller
 *				must unlock the page.
 * @param [in] 	sbi		The eMMCFS superblock information.
 * @param [in] 	pages		Pointer to locked pages.
 * @param [in] 	sector_addr	Sector address.
 * @param [in] 	page_count	Number of pages to be written.
 * @param [in] 	mode		0 - async shanpshot body pages write
 * 				1 - wait while all body pages will be written,
 * 				    and write snapshot head page in sync mode
 * @return 			Returns 0 on success, errno on failure.
 */
int emmcfs_write_snapshot_pages(struct emmcfs_sb_info *sbi, struct page **pages,
		sector_t start_sector, unsigned int page_count, int mode)
{
	struct block_device *bdev = sbi->sb->s_bdev;
	struct bio *bio;
	unsigned int count = 0;
	int continue_write = 0;
	int nr_vectr = (page_count < BIO_MAX_PAGES) ?
					page_count : BIO_MAX_PAGES;
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	struct blk_plug plug;
	blk_start_plug(&plug);
#endif

again:

#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_start_plug(&plug);
#endif
	/* Allocate a new bio */
	bio = allocate_new_bio(bdev, start_sector, nr_vectr);
	if (IS_ERR_OR_NULL(bio)) {
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
		blk_finish_plug(&plug);
#endif
		return PTR_ERR(bio);
	}
	/* Initialize the bio */
	bio->bi_private = sbi;
	if (mode)
		bio->bi_end_io = snapshot_end_head_write;
	else 
		bio->bi_end_io = snapshot_end_body_write;

	for (; count < page_count; count++)
		if ((unsigned) bio_add_page(bio, pages[count],
				PAGE_CACHE_SIZE, 0) < PAGE_CACHE_SIZE) {
			if (bio->bi_vcnt) {
				continue_write = 1;

				start_sector += (count << (PAGE_CACHE_SHIFT -
						SECTOR_SIZE_SHIFT));
			} else {
				EMMCFS_ERR("FAIL to add page to BIO");
				bio_put(bio);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
				LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
				blk_finish_plug(&plug);
#endif
				return -EFAULT;
			}
			break;
		}

	if (mode) {
		/* For writing snapshot head, we have to be sure, that all
		 * snapshot body pages have been flushed already. We will not
		 * aquire head_write_sem in write mode until there is any
		 * unflushed body pages
		 */
		down_write(&sbi->snapshot_info->head_write_sem);
		submit_bio(SUBMIT_BIO_WAIT_FOR_FLUSH_FLAGS, bio);
		/* Wait for complition of snapshot head flushing */
		down_read(&sbi->snapshot_info->head_write_sem);
		up_read(&sbi->snapshot_info->head_write_sem);
	} else {
		/* Write snapshot body pages in async mode */
		down_read(&sbi->snapshot_info->head_write_sem);
		submit_bio(WRITE_SYNC, bio);
	}

#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
		blk_finish_plug(&plug);
#endif
	if (continue_write) {
		continue_write = 0;
		goto again;
	}

	return 0;
}

/**
 * @brief 			Read page from the given sector address.
 * 				Fill the locked page with data located in the
 *				sector address. Read operation is synchronous,
 *				and caller must unlock the page.
 * @param [in] 	bdev		Pointer to block device.
 * @param [in] 	page		Pointer to locked page.
 * @param [in] 	sector_addr	Sector address.
 * @param [in] 	page_count	Number of pages to be read.
 * @return 			Returns 0 on success, errno on failure
 */
int emmcfs_read_pages(struct block_device *bdev,
			struct page **page,
			sector_t sector_addr,
			unsigned int page_count)
{
	struct bio *bio;
	struct completion wait;
	unsigned int count = 0;
	int continue_load = 0;
	int nr_vectr = (page_count < BIO_MAX_PAGES) ?
				page_count : BIO_MAX_PAGES;

#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	struct blk_plug plug;
#endif

	/* TODO : check is this necessary?*/
	/* This page can be already read by other threads */
	/*if (PageUptodate(page))
		return 0;*/
	init_completion(&wait);

again:
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_start_plug(&plug);
#endif

	/* Allocate a new bio */
	bio = allocate_new_bio(bdev, sector_addr, nr_vectr);
	if (IS_ERR_OR_NULL(bio)) {
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
			LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
		blk_finish_plug(&plug);
#endif
		return PTR_ERR(bio);
	}


	bio->bi_end_io = read_end_io;

	/* Initialize the bio */
	for (; count < page_count; count++) {

		if ((unsigned) bio_add_page(bio, page[count],
				PAGE_CACHE_SIZE, 0) < PAGE_CACHE_SIZE) {
			if (bio->bi_vcnt) {
				continue_load = 1;
				sector_addr += (count << (PAGE_CACHE_SHIFT -
						SECTOR_SIZE_SHIFT));
			} else {
				EMMCFS_ERR("FAIL to add page to BIO");
				bio_put(bio);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
				LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
				blk_finish_plug(&plug);
#endif
				return -EFAULT;
			}

			break;
		}
	}
	bio->bi_private = &wait;
	submit_bio(READ, bio);

	/* Synchronous read operation */
	wait_for_completion(&wait);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_finish_plug(&plug);
#endif
	if (continue_load) {
		continue_load = 0;
		goto again;
	}

	return 0;
}


/**
 * @brief 			Read page from the given sector address.
 * 				Fill the locked page with data located in the
 *				sector address. Read operation is synchronous,
 *				and caller must unlock the page.
 * @param [in] 	bdev		Pointer to block device.
 * @param [in] 	page		Pointer to locked page.
 * @param [in] 	sector_addr	Sector address.
 * @param [in] 	sector_count	Number of sectors to be read.
 * @param [in] 	offset		Offset value in page.
 * @return  			Returns 0 on success, errno on failure.
 */
int emmcfs_read_page(struct block_device *bdev,
			struct page *page,
			sector_t sector_addr,
			unsigned int sector_count,
			unsigned int offset)
{
	struct bio *bio;
	struct completion wait;
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	struct blk_plug plug;
#endif

	if (sector_count > SECTOR_PER_PAGE + 1)
		return -EINVAL;

	/* Allocate a new bio */
	bio = allocate_new_bio(bdev, sector_addr, 1);
	if (IS_ERR_OR_NULL(bio))
		return PTR_ERR(bio);

	/* TODO */
	/* This page can be already read by other threads */
	if (PageUptodate(page))
		return 0;
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_start_plug(&plug);
#endif
	init_completion(&wait);

	/* Initialize the bio */
	bio->bi_end_io = read_end_io;
	if ((unsigned) bio_add_page(bio, page, SECTOR_SIZE * sector_count,
			offset)	< SECTOR_SIZE * sector_count) {
		EMMCFS_ERR("FAIL to add page to BIO");
		bio_put(bio);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
		blk_finish_plug(&plug);
#endif
		return -EFAULT;
	}
	bio->bi_private = &wait;
	submit_bio(READ, bio);

	/* Synchronous read operation */
	wait_for_completion(&wait);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_finish_plug(&plug);
#endif
	return 0;
}

/**
 * @brief 			Write page to the given sector address.
 * 				Write the locked page to the sector address.
 * 				Write operation is synchronous, and caller
 *				must unlock the page.
 * @param [in] 	bdev		The eMMCFS superblock information.
 * @param [in] 	page		Pointer to locked page.
 * @param [in] 	sector_addr	Sector address.
 * @param [in] 	sector_count	Number of sector to be written.
 * @return 			Returns 0 on success, errno on failure.
 */
int emmcfs_write_page(struct block_device *bdev,
			struct page *page,
			sector_t sector_addr,
			unsigned int sector_count,
			unsigned int offset)
{
	struct bio *bio;
	struct completion wait;
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	struct blk_plug plug;
#endif

	if (sector_count > SECTOR_PER_PAGE)
		return -EINVAL;

	/* Allocate a new bio */
	bio = allocate_new_bio(bdev, sector_addr, 1);
	if (IS_ERR_OR_NULL(bio))
		return PTR_ERR(bio);

#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_start_plug(&plug);
#endif
	init_completion(&wait);

	/* Initialize the bio */
	bio->bi_end_io = end_io_write;
	if ((unsigned) bio_add_page(bio, page, SECTOR_SIZE * sector_count,
			offset) < SECTOR_SIZE * sector_count) {
		EMMCFS_ERR("FAIL to add page to BIO");
		bio_put(bio);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
		blk_finish_plug(&plug);
#endif
		return -EFAULT;
	}
	bio->bi_private = &wait;

#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	submit_bio(SWRITE_SYNC, bio);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	submit_bio(WRITE_FLUSH_FUA, bio);
#else
	BUILD_BUG();
#endif
	/* Synchronous write operation */
	wait_for_completion(&wait);
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	blk_finish_plug(&plug);
#endif
	return 0;
}
/**
 * @brief 	Structure containing necessary information to submit BIO
 *		struct. All fields except completion taken from kernel
 *		struct mpage_data.
 */
struct emmcfs_mpage_data {
	/** taken from struct mpage_data */
	struct bio *bio;
	/** taken from struct mpage_data */
	sector_t last_block_in_bio;
	/** taken from struct mpage_data */
	get_block_t *get_block;
	/** taken from struct mpage_data */
	unsigned use_writepage;
	/** completion of bio request */
	struct completion *wait;
};

/**
 * @brief 	TODO
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @return 	
 */
static struct bio *emmcfs_mpage_bio_submit(struct bio *bio,
		struct completion *wait)
{
	init_completion(wait);
	bio->bi_end_io = end_io_write;
	bio->bi_private = wait;
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	submit_bio(WRITE_BARRIER, bio);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	submit_bio(WRITE_FLUSH_FUA, bio);
#else
	BUILD_BUG();
#endif
	wait_for_completion(wait);
	return NULL;
}

/**
 * @brief 	TODO
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @return 	
 */
static struct bio *emmcfs_mpage_alloc(struct block_device *bdev,
		sector_t first_sector, int nr_vecs, gfp_t gfp_flags)
{
	struct bio *bio;

	bio = bio_alloc(gfp_flags, nr_vecs);

	if (bio == NULL && (current->flags & PF_MEMALLOC)) {
		while (!bio && (nr_vecs /= 2))
			bio = bio_alloc(gfp_flags, nr_vecs);
	}

	if (bio) {
		bio->bi_bdev = bdev;
		bio->bi_sector = first_sector;
	}
	return bio;
}

/**
 * @brief 	TODO
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @return 	
 */
static void emmcfs_write_boundary_block(struct block_device *bdev,
			sector_t bblock, unsigned blocksize)
{
	struct buffer_head *bh = __find_get_block(bdev, bblock + 1, blocksize);
	if (bh) {
		if (buffer_dirty(bh))
			ll_rw_block(WRITE, 1, &bh);
		put_bh(bh);
	}
}

/**
 * @brief 	TODO
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @return 	
 */
static int emmcfs_mpage_writepage(struct page *page,
		struct writeback_control *wbc, void *data)
{
	struct emmcfs_mpage_data *mpd = data;
	struct bio *bio = mpd->bio;
	struct address_space *mapping = page->mapping;
	struct inode *inode = page->mapping->host;
	const unsigned blkbits = inode->i_blkbits;
	unsigned long end_index;
	const unsigned blocks_per_page = PAGE_CACHE_SIZE >> blkbits;
	sector_t last_block;
	sector_t block_in_file;
	sector_t blocks[MAX_BUF_PER_PAGE];
	unsigned page_block;
	unsigned first_unmapped = blocks_per_page;
	struct block_device *bdev = NULL;
	int boundary = 0;
	sector_t boundary_block = 0;
	struct block_device *boundary_bdev = NULL;
	int length;
	struct buffer_head map_bh;
	loff_t i_size = i_size_read(inode);
	int ret = 0;

	blocks[0] = 0;
	if (page_has_buffers(page)) {
		struct buffer_head *head = page_buffers(page);
		struct buffer_head *bh = head;

		/* If they're all mapped and dirty, do it */
		page_block = 0;
		do {
			EMMCFS_BUG_ON(buffer_locked(bh));
			if (!buffer_mapped(bh)) {
				/*
				 * unmapped dirty buffers are created by
				 * __set_page_dirty_buffers -> mmapped data
				 */
				if (buffer_dirty(bh))
					goto confused;
				if (first_unmapped == blocks_per_page)
					first_unmapped = page_block;
				continue;
			}

			if (first_unmapped != blocks_per_page)
				goto confused;	/* hole -> non-hole */

			if (!buffer_dirty(bh) || !buffer_uptodate(bh))
				goto confused;
			if (page_block) {
				if (bh->b_blocknr != blocks[page_block-1] + 1)
					goto confused;
			}
			blocks[page_block++] = bh->b_blocknr;
			boundary = buffer_boundary(bh);
			if (boundary) {
				boundary_block = bh->b_blocknr;
				boundary_bdev = bh->b_bdev;
			}
			bdev = bh->b_bdev;
		} while ((bh = bh->b_this_page) != head);

		if (first_unmapped)
			goto page_is_mapped;

		/*
		 * Page has buffers, but they are all unmapped. The page was
		 * created by pagein or read over a hole which was handled by
		 * block_read_full_page().  If this address_space is also
		 * using mpage_readpages then this can rarely happen.
		 */
		goto confused;
	}

	/*
	 * The page has no buffers: map it to disk
	 */
	EMMCFS_BUG_ON(!PageUptodate(page));
	block_in_file = (sector_t)page->index << (PAGE_CACHE_SHIFT - blkbits);
	last_block = (i_size - 1) >> blkbits;
	map_bh.b_page = page;
	for (page_block = 0; page_block < blocks_per_page; ) {

		map_bh.b_state = 0;
		map_bh.b_size = 1 << blkbits;
		if (mpd->get_block(inode, block_in_file, &map_bh, 1))
			goto confused;
		if (buffer_new(&map_bh))
			unmap_underlying_metadata(map_bh.b_bdev,
						map_bh.b_blocknr);
		if (buffer_boundary(&map_bh)) {
			boundary_block = map_bh.b_blocknr;
			boundary_bdev = map_bh.b_bdev;
		}
		if (page_block) {
			if (map_bh.b_blocknr != blocks[page_block-1] + 1)
				goto confused;
		}
		blocks[page_block++] = map_bh.b_blocknr;
		boundary = buffer_boundary(&map_bh);
		bdev = map_bh.b_bdev;
		if (block_in_file == last_block)
			break;
		block_in_file++;
	}
	EMMCFS_BUG_ON(page_block == 0);

	first_unmapped = page_block;

page_is_mapped:
	end_index = i_size >> PAGE_CACHE_SHIFT;
	if (page->index >= end_index) {
		/*
		 * The page straddles i_size.  It must be zeroed out on each
		 * and every writepage invocation because it may be mmapped.
		 * "A file is mapped in multiples of the page size.  For a file
		 * that is not a multiple of the page size, the remaining memory
		 * is zeroed when mapped, and writes to that region are not
		 * written out to the file."
		 */
		unsigned offset = i_size & (PAGE_CACHE_SIZE - 1);

		if (page->index > end_index || !offset)
			goto confused;
		zero_user_segment(page, offset, PAGE_CACHE_SIZE);
	}

	/*
	 * This page will go to BIO.  Do we need to send this BIO off first?
	 */
	if (bio && mpd->last_block_in_bio != blocks[0] - 1)
		bio = emmcfs_mpage_bio_submit(bio, mpd->wait);

alloc_new:
	if (bio == NULL) {
		bio = emmcfs_mpage_alloc(bdev, blocks[0] << (blkbits - 9),
				bio_get_nr_vecs(bdev), GFP_NOFS|__GFP_HIGH);
		if (bio == NULL)
			goto confused;
	}

	/*
	 * Must try to add the page before marking the buffer clean or
	 * the confused fail path above (OOM) will be very confused when
	 * it finds all bh marked clean (i.e. it will not write anything)
	 */
	length = first_unmapped << blkbits;
	if (bio_add_page(bio, page, length, 0) < length) {
		bio = emmcfs_mpage_bio_submit(bio, mpd->wait);
		goto alloc_new;
	}

	/*
	 * OK, we have our BIO, so we can now mark the buffers clean.  Make
	 * sure to only clean buffers which we know we'll be writing.
	 */
	if (page_has_buffers(page)) {
		struct buffer_head *head = page_buffers(page);
		struct buffer_head *bh = head;
		unsigned buffer_counter = 0;

		do {
			if (buffer_counter++ == first_unmapped)
				break;
			clear_buffer_dirty(bh);
			bh = bh->b_this_page;
		} while (bh != head);

		/*
		 * we cannot drop the bh if the page is not uptodate
		 * or a concurrent readpage would fail to serialize with the bh
		 * and it would read from disk before we reach the platter.
		 */

		/*TODO buffer_heads_over_limit is almost always equal to 0
		 * but it's necessary to check this strictly
		 */
/*		if (buffer_heads_over_limit && PageUptodate(page))
			try_to_free_buffers(page);*/
	}

	EMMCFS_BUG_ON(PageWriteback(page));
	set_page_writeback(page);
	unlock_page(page);
	if (boundary || (first_unmapped != blocks_per_page)) {
		bio = emmcfs_mpage_bio_submit(bio, mpd->wait);
		if (boundary_block) {
			emmcfs_write_boundary_block(boundary_bdev,
					boundary_block, 1 << blkbits);
		}
	} else {
		mpd->last_block_in_bio = blocks[blocks_per_page - 1];
	}
	goto out;

confused:
	if (bio)
		bio = emmcfs_mpage_bio_submit(bio, mpd->wait);

	if (mpd->use_writepage) {
		ret = mapping->a_ops->writepage(page, wbc);
	} else {
		ret = -EAGAIN;
		goto out;
	}
	/*
	 * The caller has a ref on the inode, so *mapping is stable
	 */
	mapping_set_error(mapping, ret);
out:
	mpd->bio = bio;
	return ret;
}

/**
 * @brief 	TODO
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @param [in] 	
 * @return 	
 */
int emmcfs_mpage_writepages(struct address_space *mapping,
		struct writeback_control *wbc)
{
	int ret;
	struct completion wait;

	struct emmcfs_mpage_data mpd = {
			.bio = NULL,
			.last_block_in_bio = 0,
			.get_block = emmcfs_get_block,
			.use_writepage = 1,
			.wait = &wait,
	};

	ret = write_cache_pages(mapping, wbc, emmcfs_mpage_writepage, &mpd);
	if (mpd.bio)
		emmcfs_mpage_bio_submit(mpd.bio, &wait);
	return ret;
}
