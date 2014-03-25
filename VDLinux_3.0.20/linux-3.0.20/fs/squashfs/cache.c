/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * cache.c
 */

/*
 * Blocks in Squashfs are compressed.  To avoid repeatedly decompressing
 * recently accessed data Squashfs uses two small metadata and fragment caches.
 *
 * This file implements a generic cache implementation used for both caches,
 * plus functions layered ontop of the generic cache implementation to
 * access the metadata and fragment caches.
 *
 * To avoid out of memory and fragmentation issues with vmalloc the cache
 * uses sequences of kmalloced PAGE_CACHE_SIZE buffers.
 *
 * It should be noted that the cache is not used for file datablocks, these
 * are decompressed and cached in the page-cache in the normal way.  The
 * cache is only used to temporarily cache fragment and metadata blocks
 * which have been read as as a result of a metadata (i.e. inode or
 * directory) or fragment access.  Because metadata and fragments are packed
 * together into blocks (to gain greater compression) the read of a particular
 * piece of metadata or fragment will retrieve other metadata/fragments which
 * have been packed with it, these because of locality-of-reference may be read
 * in the near future. Temporarily caching them ensures they are available for
 * near future access without requiring an additional read and decompress.
 */

#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/pagemap.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs.h"

#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
#include <linux/gzmanager.h>
#endif

/*
 * Look-up block in cache, and increment usage count.  If not in cache, read
 * and decompress it from disk.
 */
struct squashfs_cache_entry *squashfs_cache_get(struct super_block *sb,
	struct squashfs_cache *cache, u64 block, int length)
{
	int i, n;
	struct squashfs_cache_entry *entry;

	spin_lock(&cache->lock);

	while (1) {
		for (i = 0; i < cache->entries; i++)
#ifdef CONFIG_SQUASHFS_DEBUGGER
			/* 
			   Every read operation on the squashfs is searching squashfs_cache 
			   If there are cache for request block, 
			   don't read flash device and just return the squashfs_cache.
			   Force to read again the block, if there are error.
			   It makes print squashfs error msg whe you read the block(cache) with error flag.
			 */	
			if (cache->entry[i].block == block && cache->entry[i].error >= 0)
#else
			if (cache->entry[i].block == block)
#endif
				break;

		if (i == cache->entries) {
			/*
			 * Block not in cache, if all cache entries are used
			 * go to sleep waiting for one to become available.
			 */
			if (cache->unused == 0) {
				cache->num_waiters++;
				spin_unlock(&cache->lock);
				wait_event(cache->wait_queue, cache->unused);
				spin_lock(&cache->lock);
				cache->num_waiters--;
				continue;
			}

			/*
			 * At least one unused cache entry.  A simple
			 * round-robin strategy is used to choose the entry to
			 * be evicted from the cache.
			 */
			i = cache->next_blk;
			for (n = 0; n < cache->entries; n++) {
				if (cache->entry[i].refcount == 0)
					break;
				i = (i + 1) % cache->entries;
			}

			cache->next_blk = (i + 1) % cache->entries;
			entry = &cache->entry[i];

			/*
			 * Initialise chosen cache entry, and fill it in from
			 * disk.
			 */
			cache->unused--;
			entry->block = block;
			entry->refcount = 1;
			entry->pending = 1;
			entry->num_waiters = 0;
			entry->error = 0;
			spin_unlock(&cache->lock);

#ifdef CONFIG_GZMANAGER_DECOMPRESS
			entry->length = squashfs_read_data(sb, entry->data, entry->ibuff,
				block, length, &entry->next_index,
				cache->block_size, cache->pages);
#else
			entry->length = squashfs_read_data(sb, entry->data,
				block, length, &entry->next_index,
				cache->block_size, cache->pages);
#endif

			spin_lock(&cache->lock);

			if (entry->length < 0)
				entry->error = entry->length;

			entry->pending = 0;

			/*
			 * While filling this entry one or more other processes
			 * have looked it up in the cache, and have slept
			 * waiting for it to become available.
			 */
			if (entry->num_waiters) {
				spin_unlock(&cache->lock);
				wake_up_all(&entry->wait_queue);
			} else
				spin_unlock(&cache->lock);

			goto out;
		}

		/*
		 * Block already in cache.  Increment refcount so it doesn't
		 * get reused until we're finished with it, if it was
		 * previously unused there's one less cache entry available
		 * for reuse.
		 */
		entry = &cache->entry[i];
		if (entry->refcount == 0)
			cache->unused--;
		entry->refcount++;

		/*
		 * If the entry is currently being filled in by another process
		 * go to sleep waiting for it to become available.
		 */
		if (entry->pending) {
			entry->num_waiters++;
			spin_unlock(&cache->lock);
			wait_event(entry->wait_queue, !entry->pending);
		} else
			spin_unlock(&cache->lock);

		goto out;
	}

out:
	TRACE("Got %s %d, start block %lld, refcount %d, error %d\n",
		cache->name, i, entry->block, entry->refcount, entry->error);

	if (entry->error)
		ERROR("Unable to read %s cache entry [%llx]\n", cache->name,
							block);
	return entry;
}


/*
 * Release cache entry, once usage count is zero it can be reused.
 */
void squashfs_cache_put(struct squashfs_cache_entry *entry)
{
	struct squashfs_cache *cache = entry->cache;

	spin_lock(&cache->lock);
	entry->refcount--;
	if (entry->refcount == 0) {
		cache->unused++;
		/*
		 * If there's any processes waiting for a block to become
		 * available, wake one up.
		 */
		if (cache->num_waiters) {
			spin_unlock(&cache->lock);
			wake_up(&cache->wait_queue);
			return;
		}
	}
	spin_unlock(&cache->lock);
}


/*
 * Delete cache reclaiming all kmalloced buffers.
 */
#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
void squashfs_cache_delete(struct squashfs_cache *cache, int gz_flag)
#else
void squashfs_cache_delete(struct squashfs_cache *cache)
#endif
{
	int i, j;

	if (cache == NULL)
		return;

	for (i = 0; i < cache->entries; i++) {
		if (cache->entry[i].data) {
#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
            if (gz_flag & useGZIP) {
                if(cache->entry[i].data[0])
                    kfree(cache->entry[i].data[0]);
            } else if (gz_flag & useZLIB) {
#endif
			for (j = 0; j < cache->pages; j++)
				kfree(cache->entry[i].data[j]);
#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
            }
#endif 
			kfree(cache->entry[i].data);
		}
#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
		if (cache->entry[i].ibuff) {
            free_pages_exact(page_address(cache->entry[i].ibuff[0]), 
                             (cache->pages + 1) * PAGE_CACHE_SIZE);
			kfree(cache->entry[i].ibuff);
	}
#else
		if (cache->entry[i].ibuff)
		{
			if (gz_flag & useGZIP) {	
				kfree(cache->entry[i].ibuff);
			} else if (gz_flag & useZLIB) {
				vfree(cache->entry[i].ibuff);
			}
			
		}
#endif
#endif
	}

	kfree(cache->entry);
	kfree(cache);
}


/*
 * Initialise cache allocating the specified number of entries, each of
 * size block_size.  To avoid vmalloc fragmentation issues each entry
 * is allocated as a sequence of kmalloced PAGE_CACHE_SIZE buffers.
 */
#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
struct squashfs_cache *squashfs_cache_init(char *name, int entries,
	int block_size, int gz_flag)
#else
struct squashfs_cache *squashfs_cache_init(char *name, int entries,
	int block_size)
#endif
{
	int i, j;
	struct squashfs_cache *cache = kzalloc(sizeof(*cache), GFP_KERNEL);

	if (cache == NULL) {
		ERROR("Failed to allocate %s cache\n", name);
		return NULL;
	}

	cache->entry = kcalloc(entries, sizeof(*(cache->entry)), GFP_KERNEL);
	if (cache->entry == NULL) {
		ERROR("Failed to allocate %s cache\n", name);
		goto cleanup;
	}

	cache->next_blk = 0;
	cache->unused = entries;
	cache->entries = entries;
	cache->block_size = block_size;
	cache->pages = block_size >> PAGE_CACHE_SHIFT;
	cache->pages = cache->pages ? cache->pages : 1;
	cache->name = name;
	cache->num_waiters = 0;
	spin_lock_init(&cache->lock);
	init_waitqueue_head(&cache->wait_queue);

	for (i = 0; i < entries; i++) {
#ifdef CONFIG_SQUASHFS_INCL_BIO
        char * page_addr;
#endif
		struct squashfs_cache_entry *entry = &cache->entry[i];

		init_waitqueue_head(&cache->entry[i].wait_queue);
		entry->cache = cache;
		entry->block = SQUASHFS_INVALID_BLK;
		entry->data = kcalloc(cache->pages, sizeof(void *), GFP_KERNEL);
		if (entry->data == NULL) {
			ERROR("Failed to allocate %s cache entry\n", name);
			goto cleanup;
		}

#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
        entry->ibuff = kcalloc((cache->pages + 1), sizeof(void *), GFP_KERNEL);
        if (entry->ibuff == NULL) {
            ERROR("Failed to allocate %s cache entry\n", name);
            goto cleanup;
        }
        page_addr = (char *)alloc_pages_exact(((cache->pages + 1) * PAGE_CACHE_SIZE), GFP_KERNEL);
        if (!page_addr) {
            ERROR("alloc_pages_exact: Failed to allocate %s buffer\n", name);
            goto cleanup;
        }
        for (j=0;j < (cache->pages+1); j++, page_addr += PAGE_CACHE_SIZE) {
            entry->ibuff[j] = virt_to_page(page_addr);
            ClearPageUptodate(entry->ibuff[j]);
#ifdef SQUASHFS_DEBUG
            printk("[%s] %s [%d] page[%d] p:%#x v.a: %#x\n",__FUNCTION__, cache->name,i,j,
                       (unsigned int) entry->ibuff[j], (unsigned int)page_address(entry->ibuff[j])); 
#endif
		}
#else
		if(gz_flag & useZLIB) {
			entry->ibuff = vmalloc(block_size + PAGE_CACHE_SIZE);
                        if(NULL == entry->ibuff){
                    ERROR("[%s] Error allocating the ibuff \n",__FUNCTION__);
                                goto cleanup;
                        }
		}
		else if ( gz_flag & useGZIP) {
			entry->ibuff = kmalloc(block_size + PAGE_CACHE_SIZE,GFP_KERNEL);
			if(NULL == entry->ibuff){
	    	    ERROR("[%s] Error allocating the ibuff \n",__FUNCTION__);
				goto cleanup;
			}
		} 
		memset(entry->ibuff,0,block_size + PAGE_CACHE_SIZE);
#endif
#endif

#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
        if(gz_flag & useZLIB) {
            /*ZLIB requires non contiguous pages */
#endif
		for (j = 0; j < cache->pages; j++) {
			entry->data[j] = kmalloc(PAGE_CACHE_SIZE, GFP_KERNEL);
			if (entry->data[j] == NULL) {

#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
                    ERROR("[zlib]Failed to allocate %s buffer\n", name);
                    goto cleanup;
                }
            }
        } else if ( gz_flag & useGZIP) {
            /* 
             * GZIP requires a contiguous pages equal to block 
             * size as output buffer 
             */
            entry->data[0] = kmalloc(PAGE_CACHE_SIZE * cache->pages, GFP_KERNEL);
            if (entry->data[0] == NULL) {
                    ERROR("[gzip] Failed to allocate %s output buffer"
                         " = %ld bytes \n",name, PAGE_CACHE_SIZE * cache->pages);
                    goto cleanup;
            }
            for (j = 1; j < cache->pages; j++) {
                entry->data[j] = ((char *)(entry->data[0])) +  
                                                        (j*PAGE_CACHE_SIZE);
                if (entry->data[j] == NULL) {
#endif
				ERROR("Failed to allocate %s buffer\n", name);
				goto cleanup;
			}
		}
	}
#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
    }
#endif
	return cache;

cleanup:
#ifdef CONFIG_SQUASHFS_GZIP_LINEAR_MEM
	squashfs_cache_delete(cache, gz_flag);
#else
	squashfs_cache_delete(cache);
#endif
	return NULL;
}


/*
 * Copy up to length bytes from cache entry to buffer starting at offset bytes
 * into the cache entry.  If there's not length bytes then copy the number of
 * bytes available.  In all cases return the number of bytes copied.
 */
int squashfs_copy_data(void *buffer, struct squashfs_cache_entry *entry,
		int offset, int length)
{
	int remaining = length;

	if (length == 0)
		return 0;
	else if (buffer == NULL)
		return min(length, entry->length - offset);

	while (offset < entry->length) {
		void *buff = entry->data[offset / PAGE_CACHE_SIZE]
				+ (offset % PAGE_CACHE_SIZE);
		int bytes = min_t(int, entry->length - offset,
				PAGE_CACHE_SIZE - (offset % PAGE_CACHE_SIZE));

		if (bytes >= remaining) {
			memcpy(buffer, buff, remaining);
			remaining = 0;
			break;
		}

		memcpy(buffer, buff, bytes);
		buffer += bytes;
		remaining -= bytes;
		offset += bytes;
	}

	return length - remaining;
}


/*
 * Read length bytes from metadata position <block, offset> (block is the
 * start of the compressed block on disk, and offset is the offset into
 * the block once decompressed).  Data is packed into consecutive blocks,
 * and length bytes may require reading more than one block.
 */
int squashfs_read_metadata(struct super_block *sb, void *buffer,
		u64 *block, int *offset, int length)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	int bytes, copied = length;
	struct squashfs_cache_entry *entry;

	TRACE("Entered squashfs_read_metadata [%llx:%x]\n", *block, *offset);

	while (length) {
		entry = squashfs_cache_get(sb, msblk->block_cache, *block, 0);
		if (entry->error)
			return entry->error;
		else if (*offset >= entry->length)
			return -EIO;

		bytes = squashfs_copy_data(buffer, entry, *offset, length);
		if (buffer)
			buffer += bytes;
		length -= bytes;
		*offset += bytes;

		if (*offset == entry->length) {
			*block = entry->next_index;
			*offset = 0;
		}

		squashfs_cache_put(entry);
	}

	return copied;
}


/*
 * Look-up in the fragmment cache the fragment located at <start_block> in the
 * filesystem.  If necessary read and decompress it from disk.
 */
struct squashfs_cache_entry *squashfs_get_fragment(struct super_block *sb,
				u64 start_block, int length)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;

	return squashfs_cache_get(sb, msblk->fragment_cache, start_block,
		length);
}


/*
 * Read and decompress the datablock located at <start_block> in the
 * filesystem.  The cache is used here to avoid duplicating locking and
 * read/decompress code.
 */
struct squashfs_cache_entry *squashfs_get_datablock(struct super_block *sb,
				u64 start_block, int length)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;

	return squashfs_cache_get(sb, msblk->read_page, start_block, length);
}


/*
 * Read a filesystem table (uncompressed sequence of bytes) from disk
 */
void *squashfs_read_table(struct super_block *sb, u64 block, int length)
{
	int pages = (length + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	int i, res;
	void *table, *buffer, **data;
#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
    char ** idata;
    char * page_addr, *pa ;
#endif
#endif

	table = buffer = kmalloc(length, GFP_KERNEL);
	if (table == NULL)
		return ERR_PTR(-ENOMEM);

	data = kcalloc(pages, sizeof(void *), GFP_KERNEL);
	if (data == NULL) {
		res = -ENOMEM;
		goto failed;
	}

#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO

	idata = kcalloc(pages+1, sizeof(void *), GFP_KERNEL);
    pa = page_addr=  (char *)alloc_pages_exact(length, GFP_KERNEL);
	if (idata == NULL || !page_addr)
		return ERR_PTR(-ENOMEM);

    for (i=0; i<pages+1; i++, page_addr += PAGE_CACHE_SIZE)
        idata[i] = (void *)virt_to_page(page_addr);
#endif
#endif

	for (i = 0; i < pages; i++, buffer += PAGE_CACHE_SIZE)
		data[i] = buffer;

#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
	res = squashfs_read_data(sb, data, idata, block, length |
		SQUASHFS_COMPRESSED_BIT_BLOCK, NULL, length, pages);
#else
	res = squashfs_read_data(sb, data, NULL, block, length |
		SQUASHFS_COMPRESSED_BIT_BLOCK, NULL, length, pages);
#endif
#else
	res = squashfs_read_data(sb, data, block, length |
		SQUASHFS_COMPRESSED_BIT_BLOCK, NULL, length, pages);
#endif

	kfree(data);
#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
	if(idata){
    	free_pages_exact(pa, length);
		kfree(idata);
	}
#endif
#endif

	if (res < 0)
		goto failed;

	return table;

failed:
	kfree(table);
	return ERR_PTR(res);
}
