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
 * block.c
 */

/*
 * This file implements the low-level routines to read and decompress
 * datablocks and metadata blocks.
 */

#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/buffer_head.h>
#ifdef CONFIG_GZMANAGER_DECOMPRESS
#include <linux/gzmanager.h>
#endif // CONFIG_GZMANAGER_DECOMPRESS 
#include <linux/sched.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs.h"
#include "decompressor.h"
#ifdef CONFIG_SQUASHFS_INCL_BIO
#include <linux/bio.h>
#endif

#if defined(CONFIG_SQUASHFS_DEBUGGER_AUTO_DIAGNOSE) && defined(CONFIG_ARM)
#include <asm/io.h>
#include <asm/mach/map.h>
#endif

#ifdef CONFIG_SQUASHFS_INCL_BIO
/* list of chained bios */
struct list_bio {
struct list_head list;
struct bio *bio;
};
#endif

/*
 * Read the metadata block length, this is stored in the first two
 * bytes of the metadata block.
 */
static struct buffer_head *get_block_length(struct super_block *sb,
			u64 *cur_index, int *offset, int *length)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	struct buffer_head *bh;

	bh = sb_bread(sb, *cur_index);
	if (bh == NULL)
		return NULL;
#ifdef CONFIG_GZMANAGER_DECOMPRESS
	if(msblk->client->flags & useGZIP){
		if (msblk->devblksize - *offset == 1) {
			*length = (unsigned char) bh->b_data[*offset];
			put_bh(bh);
			bh = sb_bread(sb, ++(*cur_index));
			if (bh == NULL)
				return NULL;
			*length |= (unsigned char) bh->b_data[0] << 8;
			*offset = 7;
		} else {
			*length = (unsigned char) bh->b_data[*offset] |
				(unsigned char) bh->b_data[*offset + 1] << 8;
			*offset += 2;

			if (msblk->devblksize - *offset < 6) {
				put_bh(bh);
				bh = sb_bread(sb, ++(*cur_index));
				if (bh == NULL)
					return NULL;
				*offset = (6 - (msblk->devblksize - *offset));

			}else{
				*offset += 6;

				if (*offset == msblk->devblksize) {
					put_bh(bh);
					bh = sb_bread(sb, ++(*cur_index));
					if (bh == NULL)
						return NULL;
					*offset = 0;
				}
			}
		}
	}else{
		if (msblk->devblksize - *offset == 1) {
			*length = (unsigned char) bh->b_data[*offset];
			put_bh(bh);
			bh = sb_bread(sb, ++(*cur_index));
			if (bh == NULL)
				return NULL;
			*length |= (unsigned char) bh->b_data[0] << 8;
			*offset = 1;
		} else {
			*length = (unsigned char) bh->b_data[*offset] |
				(unsigned char) bh->b_data[*offset + 1] << 8;
			*offset += 2;

			if (*offset == msblk->devblksize) {
				put_bh(bh);
				bh = sb_bread(sb, ++(*cur_index));
				if (bh == NULL)
					return NULL;
				*offset = 0;
			}
		}
	}
#else
	if (msblk->devblksize - *offset == 1) {
		*length = (unsigned char) bh->b_data[*offset];
		put_bh(bh);
		bh = sb_bread(sb, ++(*cur_index));
		if (bh == NULL)
			return NULL;
		*length |= (unsigned char) bh->b_data[0] << 8;
		*offset = 1;
	} else {
		*length = (unsigned char) bh->b_data[*offset] |
			(unsigned char) bh->b_data[*offset + 1] << 8;
		*offset += 2;

		if (*offset == msblk->devblksize) {
			put_bh(bh);
			bh = sb_bread(sb, ++(*cur_index));
			if (bh == NULL)
				return NULL;
			*offset = 0;
		}
	}
#endif
	return bh;
}

#ifdef CONFIG_GZMANAGER_DECOMPRESS
int squashfs_end_decomp(struct gzM_queue_data *data)
{
	int length = -1;
#ifndef CONFIG_GZMANAGER_DIRECT_CALL
	struct gzM_client *client = data->client;
#endif
	if(NULL != data){
		length = data->error;
		data->state = 1;
#ifndef CONFIG_GZMANAGER_DIRECT_CALL
		wake_up_interruptible(&client->wait_queue);
#endif // ASYNC CALL
	}else{
		printk(KERN_EMERG"ERROR: the data is NULL \n");
	}
	return length;
}
#ifdef CONFIG_SQUASHFS_INCL_BIO
struct gzM_queue_data *squashfs_data_prep(struct gzM_client *client,
	struct squashfs_sb_info *msblk,void **ibuffer, void **obuffer, 
	int b, int offset, int length, int pages)
{   
	int t =0;
	int index =0;
    struct gzM_queue_data *data = NULL;

    if(!client){
        printk(KERN_EMERG"[%s]\n",__FUNCTION__);
        return  NULL;
    }   
    data = kmalloc(sizeof(struct gzM_queue_data),GFP_KERNEL);
    if(NULL == data){
        printk(KERN_EMERG"[%s] Error allocating the data \n",__FUNCTION__);
        return NULL;
    }  

	if(NULL == page_address(ibuffer[0])){
        printk(KERN_EMERG"[%s] Error: Invalid iBuffer \n",__FUNCTION__);
		kfree(data);
		return NULL;
	} 

	for (t = 0; t < b; t++){
		wait_on_page_update(ibuffer[t], &msblk->wq);
    }

	data->buff = page_address(ibuffer[0]);
	data->opages = obuffer;
	data->offset = offset;
	data->client = client;
	data->length = length;
	data->npages = pages;
	data->end_decomp = squashfs_end_decomp;
	data->state = 0;
	data->error = 0;


   	return data; 
} 

#else
struct gzM_queue_data *squashfs_data_prep(struct gzM_client *client,
    void **buffer, char * ibuff, struct buffer_head **bh, int b, int offset, 
	int length, int pages)
{   
	int t =0;
    struct gzM_queue_data *data = NULL;
	int index =0;

    if(!client){
        printk(KERN_EMERG"[%s]\n",__FUNCTION__);
        return  NULL;
    }   
    data = kmalloc(sizeof(struct gzM_queue_data),GFP_KERNEL);
    if(NULL == data){
        printk(KERN_EMERG"[%s] Error allocating the data \n",__FUNCTION__);
        return NULL;
    }  

	data->buff = ibuff;

	for (t = 0,index=0; t < b; t++) {
		wait_on_buffer(bh[t]);
		if (!buffer_uptodate(bh[t]))
			goto bh_release;
		memcpy(data->buff+index,bh[t]->b_data,PAGE_CACHE_SIZE);
		index += PAGE_CACHE_SIZE;
	}

	data->opages = kcalloc(pages, sizeof(void *), GFP_KERNEL);
    if (data->opages == NULL) {
		if(data)
			kfree(data);
		return NULL;
    }

	for (t = 0; t < pages; t++) {
		data->opages[t] = buffer[t];
	}
	data->offset = offset;
	data->client = client;
	data->length = length;
	data->npages = pages;
	data->end_decomp = squashfs_end_decomp;
	data->state = 0;
	data->error = 0;


   	return data; 
bh_release:
	for (; t < b; t++)
		put_bh(bh[t]);
	if(data)
		kfree(data);
	return NULL;
} 
#endif

#ifndef CONFIG_SQUASHFS_INCL_BIO
void squashfs_put_bh(struct buffer_head **bh, int b)
{
	int t;
    //printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	
	for(t=0; t< b; t++){
		put_bh(bh[t]);
	}
}
#endif // CONFIG_SQUASHFS_INCL_BIO
#endif //(CONFIG_GZMANAGER_DECOMPRESS)

#ifdef CONFIG_SQUASHFS_DEBUGGER

extern char last_file_name[100];

#define	DATABLOCK	0
#define	METABLOCK	1
#define	VERSION_INFO	"Ver 2.01"

static	DEFINE_SPINLOCK(squashfs_dump_lock);

u64	__cur_index;
int	__offset;
int	__length;

/*
 * Dump out the contents of some memory nicely...
 */
static void dump_mem_be(const char *lvl, const char *str, unsigned long bottom,
		unsigned long top)
{
	unsigned long first;
	mm_segment_t fs;
	int i;

	/*
	 * We need to switch to kernel mode so that we can use __get_user
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);

	printk("%s%s(0x%08lx to 0x%08lx)\n", lvl, str, bottom, top);

	for (first = bottom & ~31; first < top; first += 32) {
		unsigned long p;
		char str[sizeof(" 12345678") * 8 + 1];

		memset(str, ' ', sizeof(str));
		str[sizeof(str) - 1] = '\0';

		for (p = first, i = 0; i < 8 && p <= top; i++, p += 4) {
			if (p >= bottom && p <= top) {
				unsigned long val;
				if (__get_user(val, (unsigned long *)p) == 0)
				{
					val = __cpu_to_be32(val);
					sprintf(str + i * 9, " %08lx", val);
				}
				else
					sprintf(str + i * 9, " ????????");
			}
		}
		printk("%s%04lx:%s\n", lvl, first & 0xffff, str);
	}

	set_fs(fs);
}

int sp_memcmp(const void *cs, const void *ct, size_t count, int* offset)
{
	const unsigned char *su1, *su2;
	int res = 0;

	printk("memcmp :  0x%p(0x%08x) <-> 0x%p(0x%08x) \n",
			cs,*(unsigned int*)cs,ct,*(unsigned int*)ct);

	*offset = count;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;

	*offset = *offset - count;

	return res;
}

#endif

#ifdef CONFIG_SQUASHFS_INCL_BIO
static void bio_end_io_read(struct bio *bio, int err)
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
	struct bio_vec *bvec = bio->bi_io_vec; 
    wait_queue_head_t * wq = (wait_queue_head_t *) bio->bi_private;

	do {
		struct page *page = bvec->bv_page;
		if (uptodate) {
            SetPageUptodate(page);
            if(waitqueue_active(wq))
                wake_up (wq);
		} else {
			printk(KERN_EMERG"[%s] Page not uptodate [%d]0x#x \n",__FUNCTION__,\
				page->index,page);
			ClearPageUptodate(page);
			SetPageError(page);
		}

	} while (++bvec <= bio->bi_io_vec + bio->bi_vcnt - 1);

    bio_put(bio);
}
#endif

/*
 * Read and decompress a metadata block or datablock.  Length is non-zero
 * if a datablock is being read (the size is stored elsewhere in the
 * filesystem), otherwise the length is obtained from the first two bytes of
 * the metadata block.  A bit in the length field indicates if the block
 * is stored uncompressed in the filesystem (usually because compression
 * generated a larger block - this does occasionally happen with zlib).
 */
#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
int squashfs_read_data(struct super_block *sb, void **buffer, void **ibuffer,u64 index,
			int length, u64 *next_index, int srclength, int pages)
#else
int squashfs_read_data(struct super_block *sb, void **buffer, char *ibuff, u64 index,
			int length, u64 *next_index, int srclength, int pages)
#endif
#else
int squashfs_read_data(struct super_block *sb, void **buffer, u64 index,
			int length, u64 *next_index, int srclength, int pages)
#endif
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	struct buffer_head **bh;
	int offset = index & ((1 << msblk->devblksize_log2) - 1);
	u64 cur_index = index >> msblk->devblksize_log2;
	int bytes, compressed = 0, b = 0, k = 0, page = 0, avail;
#ifdef CONFIG_GZMANAGER_DECOMPRESS
    struct gzM_queue_data *data = NULL;
	struct gzM_client *client = msblk->client;
#endif
#ifdef CONFIG_SQUASHFS_INCL_BIO
	struct bio *bio = NULL;
    int ret=0,c=0;
    wait_queue_head_t * wq = &msblk->wq;
    struct buffer_head *bhead;
    LIST_HEAD(list_bio_head);       /* bio list head */
    struct list_bio * tmp,*tmp2;

#endif

#ifdef CONFIG_SQUASHFS_DEBUGGER
	int block_type;     /* 0: DataBlock, 1: Metadata block */
	int all_block_read; /* 0: Read partial block , 1: Read all compressed block */
	int fail_block;
	int i;
	int old_k = 0;
	char bdev_name[BDEVNAME_SIZE];
	int block_release = 0; /*FALSE*/

	__cur_index = cur_index;
	__offset    = offset;
	__length    = length;
#elif defined(CONFIG_GZMANAGER_DIRECT_CALL)
	int block_type;     /* 0: DataBlock, 1: Metadata block */
#endif

#ifndef CONFIG_SQUASHFS_INCL_BIO
	bh = kcalloc(((srclength + msblk->devblksize - 1)
		>> msblk->devblksize_log2) + 1, sizeof(*bh), GFP_KERNEL);
	if (bh == NULL)
		return -ENOMEM;
#endif

	if (length) {
		/*
		 * Datablock.
		 */
#ifdef CONFIG_SQUASHFS_INCL_BIO
        int t =0;
#endif 
#ifdef CONFIG_SQUASHFS_DEBUGGER
		block_type = DATABLOCK;
#elif defined(CONFIG_GZMANAGER_DIRECT_CALL)
		block_type = DATABLOCK;
#endif
		bytes = -offset;
		compressed = SQUASHFS_COMPRESSED_BLOCK(length);
		length = SQUASHFS_COMPRESSED_SIZE_BLOCK(length);
		if (next_index)
			*next_index = index + length;

		TRACE("Block @ 0x%llx, %scompressed size %d, src size %d\n",
			index, compressed ? "" : "un", length, srclength);

		if (length < 0 || length > srclength ||
				(index + length) > msblk->bytes_used)
			goto read_failure;

		for (b = 0; bytes < length; b++, cur_index++) {
#ifndef CONFIG_SQUASHFS_INCL_BIO
			bh[b] = sb_getblk(sb, cur_index);
			if (bh[b] == NULL)
				goto block_release;
#endif
			bytes += msblk->devblksize;
		}
#ifdef CONFIG_SQUASHFS_INCL_BIO
        /* Max number of sectors per request at the moment is 512.
         * As a consquence, the block layer will restrict each bio to 
         * have maximum of 64 pages. We create new bio if the number of
         * new pages added in bio increases beyong 64.
         *
         */
         
	    cur_index = index >> msblk->devblksize_log2;
        do {
            struct list_bio *this_bio;
            this_bio = kmalloc( sizeof(struct list_bio *), GFP_KERNEL);
            if (unlikely(!this_bio)) {
                printk(KERN_EMERG"[squashfs] No memory for new bio list node\n"); 
                return -ENOMEM;
            }

            list_add_tail (&(this_bio->list), &(list_bio_head));
            /* allocate and initialise bio */
            this_bio->bio = bio_alloc(GFP_NOIO, (b - t)); 
            if (!this_bio->bio)
                goto fail_abort;

            bio_get(this_bio->bio);
            this_bio->bio->bi_bdev = sb->s_bdev;
            this_bio->bio->bi_sector = (cur_index * (sb->s_blocksize >> 9));
            this_bio->bio->bi_end_io =  bio_end_io_read;  
            this_bio->bio->bi_private = (void *)wq;       

            /* add page to bio */ 
            for ( ;t < b; t++) {
                ClearPageUptodate(ibuffer[t]);
                if ((bio_add_page(this_bio->bio, ibuffer[t],PAGE_CACHE_SIZE , 0) != 
                           PAGE_CACHE_SIZE) && b >= MAX_PAGES_PER_BLOCK_REQ ) {
                    break;
                }
                cur_index ++;
            }
            submit_bio(READ, this_bio->bio); 
        } while (t < b);
#else
		ll_rw_block(READ, b, bh);
#endif

	} else {
		/*
		 * Metadata block.
		 */
#ifdef CONFIG_SQUASHFS_DEBUGGER
		block_type = METABLOCK;
#elif defined(CONFIG_GZMANAGER_DIRECT_CALL)
		block_type = METABLOCK;
#endif
#ifndef CONFIG_GZMANAGER_DECOMPRESS
		if ((index + 2) > msblk->bytes_used)
			goto read_failure;
#else
		if(msblk->client->flags & useGZIP){
			if ((index + 8) > msblk->bytes_used)
				goto read_failure;
		}else{
			if ((index + 2) > msblk->bytes_used)
				goto read_failure;
		}
#endif

#ifdef CONFIG_SQUASHFS_INCL_BIO
		bhead = get_block_length(sb, &cur_index, &offset, &length);
		if (bhead == NULL)
			goto read_failure;
        /* bhead alread points to a page */
        memcpy(page_address(ibuffer[0]),bhead->b_data,PAGE_CACHE_SIZE);

        /* This page is already read from disk and is updated due to  a 
         * call to __bread(). The bhead->b_end_io function i.e 
         * end_buffer_read_sync won't set the PG_update bit. So we do it
         * ourselves 
         */
        SetPageUptodate(ibuffer[0]);
        put_bh(bhead);
#else
		bh[0] = get_block_length(sb, &cur_index, &offset, &length);
		if (bh[0] == NULL)
			goto read_failure;
#endif
		b = 1;

		bytes = msblk->devblksize - offset;
		compressed = SQUASHFS_COMPRESSED(length);
		length = SQUASHFS_COMPRESSED_SIZE(length);
#ifndef CONFIG_GZMANAGER_DECOMPRESS
		if (next_index)
			*next_index = index + length + 2;
#else
		if(msblk->client->flags & useGZIP){
			if (next_index)
				*next_index = index + length + 8;
		}else{
			if (next_index)
				*next_index = index + length + 2;
		}
#endif

		TRACE("Block @ 0x%llx, %scompressed size %d\n", index,
				compressed ? "" : "un", length);

		if (length < 0 || length > srclength ||
					(index + length) > msblk->bytes_used)
			goto block_release;

		for (; bytes < length; b++) {
#ifndef CONFIG_SQUASHFS_INCL_BIO
			bh[b] = sb_getblk(sb, ++cur_index);
			if (bh[b] == NULL)
				goto block_release;
#endif
			bytes += msblk->devblksize;
		}
#ifdef CONFIG_SQUASHFS_INCL_BIO
        if (b > 1) {
           	for (c = 1; c < b; c++) {
                bytes += PAGE_CACHE_SIZE;
            }
        
            bio = bio_alloc(GFP_NOIO, b-1); 
            if (!bio)  
                goto fail_abort;

            bio->bi_bdev = sb->s_bdev;
            bio->bi_sector = ++cur_index * (sb->s_blocksize >> 9);
            bio->bi_end_io =  bio_end_io_read;  
            bio->bi_private = (void *)wq;       

            /* associate pages with bio */
            for (i=1; i < b; i++) {
                ClearPageUptodate(ibuffer[i]);
                bio_add_page(bio, ibuffer[i], PAGE_CACHE_SIZE , 0);
            }

            bio_get(bio);
            submit_bio(READ, bio);
            if (bio_flagged(bio, BIO_EOPNOTSUPP))
                ret = -EOPNOTSUPP;
        } /* end if (b>1) */
#else
		ll_rw_block(READ, b - 1, bh + 1);
#endif

	}

	if (compressed) {
#ifdef CONFIG_GZMANAGER_DECOMPRESS
#ifdef CONFIG_SQUASHFS_INCL_BIO
		data = squashfs_data_prep(client, msblk, ibuffer, buffer, b, offset, length, pages);
#else
		data = squashfs_data_prep(client, buffer, ibuff, bh, b, offset, length,pages);
#endif // CONFIG_SQUASHFS_INCL_BIO
		if( data == NULL)
		{
			goto read_failure;
		}
#ifdef CONFIG_GZMANAGER_DIRECT_CALL
		if(block_type == METABLOCK)
		{
			length = client->hops->sw_decompress(data->buff+data->offset,length,data->opages,data->npages);
		}
		else
		{
			length = client->hops->decompress(data->buff+data->offset,length,data->opages,data->npages);
		}

#else // CONFIG_GZMANAGER_ASYNC_CALL
		data->state = 0;
		if(gzM_submit_data(data)){
			printk(KERN_EMERG"gzM_submission failed \n");
			return -1;
			// do cleanup
		}
		/* waiting for the decompression to finished 
		 * 1. Ideally we should move ahead from here
		 *    otherwise we can use the direct call if we want to wait
		 *    depending upon how many blocks we are processing
		 *    Squashfs need to be changed so currently we are waiting 
		 *    for single block to get read and get the data from the 
		 *    returned value in end_decomp function. 
	 	 */
wait_state:
		if(!(client->flags & useBLKS)){
			//while(0 >= data->error){
			//while(!data->state){
			if(!data->state){
				// We dont want to wait here other wise the purpose of submission is defeated 
				// we will remove the code from here later
				wait_event_interruptible(client->wait_queue, data->state);
				//schedule();
			}
			//spin_lock(&data->client->lock);
			if(data->state){
				length = data->error;
			}else{
				printk(KERN_EMERG"Waiting State again \n");
				//spin_unlock(&data->client->lock);
				goto wait_state;
			}
			//spin_unlock(&data->client->lock);
			//printk(KERN_EMERG"After decompression %#x %d::%d \n",data,data->error, length);
			// this will be freed in end_decomp function
			//kfree(data);
		}
		//printk(KERN_EMERG"length %d \n",length);
#endif // CONFIG_GZMANAGER_ASYNC_CALL
		if(data){

#ifndef CONFIG_SQUASHFS_INCL_BIO
			if(data->opages)
				kfree(data->opages);
#endif

			kfree(data);
		}
#ifndef CONFIG_SQUASHFS_INCL_BIO
		squashfs_put_bh(bh,b);
#endif
		//printk(KERN_EMERG"data %d \n",length);
#else // NORMAL 
		length = squashfs_decompress(msblk, buffer, bh, b, offset,
			 length, srclength, pages);
#endif // CONFIG_GZMANAGER_DECOMPRESS 
		if (length < 0)
			goto read_failure;
	} else {
		/*
		 * Block is uncompressed.
		 */
		int i, in, pg_offset = 0;

		for (i = 0; i < b; i++) {
#ifdef CONFIG_SQUASHFS_INCL_BIO
            /* If  bio_end_io_read() for page, pg[i], is scheduled earlier 
             * than wait_on_page_update(pg[i]), it will be already uptodate. 
             * We need not to issue a wait_on_page_update for such a page.
             */
              wait_on_page_update(ibuffer[i],wq);

            if (unlikely(!PageUptodate(ibuffer[i])))
				goto block_release;
#else
			wait_on_buffer(bh[i]);
			if (!buffer_uptodate(bh[i]))
				goto block_release;
#endif
		}

		for (bytes = length; k < b; k++) {
			in = min(bytes, msblk->devblksize - offset);
			bytes -= in;
			while (in) {
				if (pg_offset == PAGE_CACHE_SIZE) {
					page++;
					pg_offset = 0;
				}
				avail = min_t(int, in, PAGE_CACHE_SIZE -
						pg_offset);
#ifdef CONFIG_SQUASHFS_INCL_BIO
				memcpy(buffer[page] + pg_offset,
						page_address(ibuffer[k]) + offset, avail);
#else
				memcpy(buffer[page] + pg_offset,
						bh[k]->b_data + offset, avail);
#endif
				in -= avail;
				pg_offset += avail;
				offset += avail;
			}
			offset = 0;
#ifdef CONFIG_SQUASHFS_INCL_BIO
            if (page_has_buffers(ibuffer[k]))
                put_bh(page_buffers((struct page *) ibuffer[k]));
#else
			put_bh(bh[k]);
#endif
		}
	}

#ifdef CONFIG_SQUASHFS_INCL_BIO    
   list_for_each_entry_safe (tmp, tmp2, &list_bio_head, list) {
        list_del(&(tmp->list));
        if (tmp){
			if(tmp->bio)
				bio_put(tmp->bio);
            kfree(tmp);

		}
	}
    if (bio)
        bio_put(bio);

    for (i=0; i < b; i++){
        ClearPageUptodate(ibuffer[i]);
	}

#else

	kfree(bh);
#endif
	return length;

block_release:

#ifdef CONFIG_SQUASHFS_DEBUGGER
	block_release = 1;
#endif // CONFIG_SQUASHFS_DEBUGGER

read_failure:

#ifdef CONFIG_SQUASHFS_DEBUGGER

	old_k = k;

	if( old_k < b - 1 )     /* Failed during inflate *partial* block, so last read-block is the reason of failure */
	{
		fail_block = old_k;
		all_block_read = 0;
	}
	else                /* Failed during inflate *all* block, so We can't know what is fail_block */
	{
		fail_block = 0;
		all_block_read = 1;
	}

	preempt_disable();
	spin_lock(&squashfs_dump_lock);
	
	printk(KERN_ERR "---------------------------------------------------------------------\n");
	printk(KERN_ERR " CURRENT : %s\n",current->comm);
	printk(KERN_ERR "---------------------------------------------------------------------\n");
	printk(KERN_ERR "== [System Arch] Squashfs Debugger - %7s ======== Core : %2d ====\n"
							, VERSION_INFO, current_thread_info()->cpu);
	printk(KERN_ERR "---------------------------------------------------------------------\n");
	if(block_type == METABLOCK)         /* METABLOCK */
	{
		printk(KERN_ERR"[MetaData Block]\nBlock @ 0x%llx, %scompressed size %d, nr of b: %d\n", index,
				compressed ? "" : "un", __length, b - 1);
		if(old_k == 0)
			/* get_block_length() failure*/
			printk(KERN_ERR "- Metablock 0 is broken.. compressed block - bh[0]\n");
		else
			printk(KERN_ERR "- %s all compressed block bh[0] + (%d/%d)\n",
					all_block_read ? "Read":"Didn't read",
					all_block_read ? b-1 : old_k, b-1);
	}
	else                                /* DATABLOCK */
	{
		printk(KERN_ERR"[DataBlock]\nBlock @ 0x%llx, %scompressed size %d, src size %d, nr of bh :%d\n",
				index, compressed ? "" : "un", __length, srclength, b);
		printk(KERN_ERR "- %s all compressed block (%d/%d)\n",
				all_block_read ? "Read":"Didn't read",
				all_block_read ? b : old_k+1, b);
	}

	printk(KERN_ERR "---------------------------------------------------------------------\n");
	if( next_index )
	{
		printk(KERN_ERR "[Block: 0x%08llx(0x%08llx) ~ 0x%08llx(0x%08llx)]\n",
				index >> msblk->devblksize_log2, index,
				*next_index >> msblk->devblksize_log2, *next_index);
	}
	else
	{
		printk(KERN_ERR "[Block: 0x%08llx(0x%08llx) ~ UNDEFINED]\n",
				index >> msblk->devblksize_log2, index);
	}
	printk(KERN_ERR "\tlength : %d , device block_size : %d \n",
			__length, msblk->devblksize /*msblk->block_size*/);
	printk(KERN_ERR "---------------------------------------------------------------------\n");

	if(all_block_read)  /* First Block info */
		printk(KERN_ERR "<< First Block Info >>\n");
	else                /* Fail Block info */
		printk(KERN_ERR "<< Fail Block Info >>\n");

	printk(KERN_ERR "- bh->b_blocknr : %4llu (0x%08llx x %4d byte = 0x%08llx)\n",
			bh[fail_block]->b_blocknr,
			bh[fail_block]->b_blocknr,
			bh[fail_block]->b_size,
			bh[fail_block]->b_blocknr * bh[fail_block]->b_size);
	printk(KERN_ERR "- bi_sector     : %4llu (0x%08llx x  512 byte = 0x%08llx)\n",
			bh[fail_block]->b_blocknr * (bh[fail_block]->b_size >> 9),
			bh[fail_block]->b_blocknr * (bh[fail_block]->b_size >> 9),
			bh[fail_block]->b_blocknr * (bh[fail_block]->b_size >> 9) * 512 /* sector size = 512byte fixed */);
	printk(KERN_ERR "- bh[%d]->b_data : 0x%p\n", fail_block, bh[fail_block]->b_data);
	printk(KERN_ERR "---------------------------------------------------------------------\n");
	printk(KERN_ERR "Device : %s \n", bdevname(sb->s_bdev, bdev_name));
	printk(KERN_ERR "---------------------------------------------------------------------\n");

	if(block_type == METABLOCK) /* MetaData Block */
	{
		printk(KERN_ERR "MetaData Access Error : Maybe mount or ls problem..????\n");
	}
	else						/* Data Block */
	{
		printk(KERN_ERR " - CAUTION : Below is the information just for reference ....!!\n");
		printk(KERN_ERR " - LAST ACCESS FILE : %s \n",last_file_name);
	}

	printk(KERN_ERR "---------------------------------------------------------------------\n");

	for(i = 0 ; i < b; i++)
	{
		printk(KERN_ERR "bh[%2d]:0x%p",i,bh[i]);
		if(bh[i])
		{
			printk(" | bh[%2d]->b_data:0x%p | ",i,bh[i]->b_data);
			printk("bh value :0x%08x",
					__cpu_to_be32(*(unsigned int*)(bh[i]->b_data)));
			if(fail_block == i)
				printk("*");
		}
		printk("\n");
	}
	printk(KERN_ERR "---------------------------------------------------------------------\n");
	printk(KERN_ERR "[ Original Data Buffer ]\n");

	for(i = 0 ; i < b; i++)
	{
		printk(KERN_ERR "bh[%2d]:0x%p",i,bh[i]);
		if(bh[i])
		{
			printk(" | bh[%2d]->b_data:0x%p | ",i,bh[i]->b_data);
			dump_mem_be(KERN_ERR, "DUMP BH->b_data",
					(unsigned int)bh[i]->b_data,
					(unsigned int)bh[i]->b_data + msblk->devblksize - 4);
		}
		printk("\n");
	}

#ifdef CONFIG_SQUASHFS_DEBUGGER_AUTO_DIAGNOSE
	/* == verifying flash routine == */
	printk(KERN_ERR "---------------------------------------------------------------------\n");
	printk(KERN_ERR "[verifying flash data]\n");

	if (1)
	{
		char* block_buffer = NULL;
		char* nc_bh_bdata[32] = {0,};
		int diff_offset = 0;
		int detect_origin_vs_ncbh = 0;
		int detect_bh_vs_ncbh = 0;
		int check_unit = 0;

		if( all_block_read )
			check_unit = b;
		else
			check_unit = 1;

		/* 1. buffer allocation */
		block_buffer = kmalloc(msblk->devblksize * check_unit, GFP_KERNEL);

		if( block_buffer == NULL )
		{
			printk(KERN_EMERG "verifying flash failed - not enough memory \n");
			goto buffer_alloc_fail;
		}

		/* 2. copy original data */
		for(i = fail_block ; i < fail_block + check_unit; i++)
		{
			if( all_block_read )
				memcpy(block_buffer + i * msblk->devblksize, bh[i]->b_data, msblk->devblksize);
			else
				memcpy(block_buffer, bh[i]->b_data, msblk->devblksize);

			clear_buffer_uptodate(bh[i]);   //force to read again
		}

		/* In this stage, we don't need to flush real cache. we want to remain real problem. */
#if 0
		/* flush L1 + L2 cache */
		flush_cache_all();
#endif
		/* 3. Reread buffer block from flash */
		if( block_type == DATABLOCK )   /* Datablock */
		{
			ll_rw_block(READ, check_unit, bh);
		}
		else                            /* Metablock */
		{
			/* 1st block of Metadata is used for special purpose */
			bh[0] = get_block_length(sb, &__cur_index, &__offset, &__length);
			ll_rw_block(READ, b - 1, bh + 1);
		}

		/* 4. ioremap buffer_head to see the uncache area */
		for(i = fail_block ; i < fail_block + check_unit; i++)
		{
			//printk("%x %x %x\n",bh[i]->b_data, virt_to_phys(bh[i]->b_data),msblk->devblksize);
#if defined(CONFIG_ARM)
			nc_bh_bdata[i] = __arm_ioremap(virt_to_phys(bh[i]->b_data), msblk->devblksize, MT_UNCACHED);
#elif defined(CONFIG_MIPS)
			nc_bh_bdata[i] = ioremap(virt_to_phys(bh[i]->b_data),msblk->devblksize);
#endif
		}

		/* 5. Checking buffer : Original Data vs Reread Data (cached) vs Reread Data (uncached) */
		for(i = fail_block ; i < fail_block + check_unit; i++)
		{
			detect_origin_vs_ncbh = sp_memcmp(block_buffer + (msblk->devblksize * i), 
										nc_bh_bdata[i], msblk->devblksize, &diff_offset);

			detect_bh_vs_ncbh     = sp_memcmp(bh[i]->b_data, 
										nc_bh_bdata[i], msblk->devblksize, &diff_offset);

			if( detect_origin_vs_ncbh || detect_bh_vs_ncbh )
			{
				diff_offset = diff_offset & ~(0x3);
				fail_block = i; /* We found fail block */
				goto diagnose_detect;
			}
			diff_offset = 0;
		}

		/* 6. print the result */
		printk(KERN_ERR "---------------------------------------------------------------------\n");
		printk(KERN_ERR "[ result - Auto diagnose can't find any cause.....;;;;;;;            ]\n");
		printk(KERN_ERR "[        - flash image is broken or update is canceled abnormally??? ]\n");
		printk(KERN_ERR "---------------------------------------------------------------------\n");

		goto diagnose_end;	/* goto the end */

diagnose_detect:

		printk(KERN_ERR "bh[%2d]:0x%p | bh[%2d]->b_data:%p (physical addr)\n",
				fail_block, bh[fail_block], fail_block, nc_bh_bdata[fail_block]);

		printk(KERN_ERR "---------------------------------------------------------------------\n");
		dump_mem_be(KERN_ERR, "DUMP Fail Block ( non cached data ) ",
				(unsigned int)nc_bh_bdata[fail_block],
				(unsigned int)nc_bh_bdata[fail_block] + msblk->devblksize - 4); /* e.g. 4k */

		printk(KERN_ERR "---------------------------------------------------------------------\n");
		printk(KERN_ERR "[ verifying result - Original Data vs Reread Data (Uncached) : ");
		printk(detect_origin_vs_ncbh ? "FAIL ]\n":"PASS ]\n");
		printk(KERN_ERR "[                  - Data(cached) vs Data(Uncached)          : ");
		printk(detect_bh_vs_ncbh ? "FAIL ]\n":"PASS ]\n");
		printk(KERN_ERR "---------------------------------------------------------------------\n");
		printk(KERN_ERR "[ verifying result - Cache and Memory data is different.........!!  ]\n");
		printk(KERN_ERR "---------------------------------------------------------------------\n");

		printk(KERN_ERR "         Original   -- Cached Data -- Non cached Data \n");
		printk(KERN_ERR " Addr :  0x%8p -- 0x%8p  -- 0x%8p \n",
				block_buffer + diff_offset,
				bh[fail_block]->b_data  + diff_offset,
				nc_bh_bdata[fail_block] + diff_offset);
		printk(KERN_ERR " Value:  0x%08x -- 0x%08x  -- 0x%08x  (big endian)\n",
				__cpu_to_be32(*(unsigned int*)(block_buffer + (fail_block * msblk->devblksize) + diff_offset)),
				__cpu_to_be32(*(unsigned int*)(bh[fail_block]->b_data  + diff_offset)),
				__cpu_to_be32(*(unsigned int*)(nc_bh_bdata[fail_block] + diff_offset)));

diagnose_end:

		/* 7. unmap mapping are for uncached  */
		for(i = fail_block ; i < fail_block + check_unit; i++)
		{
			if(nc_bh_bdata[i] != NULL)	
				iounmap(nc_bh_bdata[i]);
		}

		kfree(block_buffer);

buffer_alloc_fail:
		printk(KERN_ERR "=====================================================================\n");

	}

#endif /* end of CONFIG_SQUASHFS_DEBUGGER_AUTO_DIAGNOSE */

	spin_unlock(&squashfs_dump_lock);
	preempt_enable();
	
#endif /* end of CONFIG_SQUASHFS_DEBUGGER */


	if(block_release == 1)
	{
		for (; k < b; k++)
			put_bh(bh[k]);
	}

	ERROR("squashfs_read_data failed to read block 0x%llx\n",
					(unsigned long long) index);
	kfree(bh);

	return -EIO;

#ifdef CONFIG_SQUASHFS_INCL_BIO
fail_abort:
	ERROR("failed to get free page for index 0x%llx \n",cur_index);
    return -EIO;
#endif
}

