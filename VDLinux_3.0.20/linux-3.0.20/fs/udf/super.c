/*
 * super.c
 *
 * PURPOSE
 *  Super block routines for the OSTA-UDF(tm) filesystem.
 *
 * DESCRIPTION
 *  OSTA-UDF(tm) = Optical Storage Technology Association
 *  Universal Disk Format.
 *
 *  This code is based on version 2.00 of the UDF specification,
 *  and revision 3 of the ECMA 167 standard [equivalent to ISO 13346].
 *    http://www.osta.org/
 *    http://www.ecma.ch/
 *    http://www.iso.org/
 *
 * COPYRIGHT
 *  This file is distributed under the terms of the GNU General Public
 *  License (GPL). Copies of the GPL can be obtained from:
 *    ftp://prep.ai.mit.edu/pub/gnu/GPL
 *  Each contributing author retains all rights to their own work.
 *
 *  (C) 1998 Dave Boynton
 *  (C) 1998-2004 Ben Fennema
 *  (C) 2000 Stelias Computing Inc
 *
 * HISTORY
 *
 *  09/24/98 dgb  changed to allow compiling outside of kernel, and
 *                added some debugging.
 *  10/01/98 dgb  updated to allow (some) possibility of compiling w/2.0.34
 *  10/16/98      attempting some multi-session support
 *  10/17/98      added freespace count for "df"
 *  11/11/98 gr   added novrs option
 *  11/26/98 dgb  added fileset,anchor mount options
 *  12/06/98 blf  really hosed things royally. vat/sparing support. sequenced
 *                vol descs. rewrote option handling based on isofs
 *  12/20/98      find the free space bitmap (if it exists)
 */

#include "udfdecl.h"

#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/parser.h>
#include <linux/stat.h>
#include <linux/cdrom.h>
#include <linux/nls.h>
#include <linux/buffer_head.h>
#include <linux/vfs.h>
#include <linux/vmalloc.h>
#include <linux/errno.h>
#include <linux/mount.h>
#include <linux/seq_file.h>
#include <linux/bitmap.h>
#include <linux/crc-itu-t.h>
#include <asm/byteorder.h>
#include "ecma_167.h"
#include "udf_sb.h"
#include "udf_i.h"

#include <linux/init.h>
#include <asm/uaccess.h>

#define VDS_POS_PRIMARY_VOL_DESC	0
#define VDS_POS_UNALLOC_SPACE_DESC	1
#define VDS_POS_LOGICAL_VOL_DESC	2
#define VDS_POS_PARTITION_DESC		3
#define VDS_POS_IMP_USE_VOL_DESC	4
#define VDS_POS_VOL_DESC_PTR		5
#define VDS_POS_TERMINATING_DESC	6
#define VDS_POS_LENGTH			7

#define UDF_DEFAULT_BLOCKSIZE 2048

static char error_buf[1024];
static int timeouts    = 0;
static int read_errors = 0;

/* These are the "meat" - everything else is stuffing */
static int udf_fill_super(struct super_block *, void *, int);
static void udf_put_super(struct super_block *);
static int udf_sync_fs(struct super_block *, int);
static int udf_remount_fs(struct super_block *, int *, char *);
static void udf_load_logicalvolint(struct super_block *, struct kernel_extent_ad);
static int udf_find_fileset(struct super_block *, struct kernel_lb_addr *,
			    struct kernel_lb_addr *);
static void udf_load_fileset(struct super_block *, struct buffer_head *,
			     struct kernel_lb_addr *);
static void udf_open_lvid(struct super_block *);
static void udf_close_lvid(struct super_block *);
static unsigned int udf_count_free(struct super_block *);
static int udf_statfs(struct dentry *, struct kstatfs *);
static int udf_show_options(struct seq_file *, struct vfsmount *);
static void udf_error(struct super_block *sb, const char *function,
		      const char *fmt, ...);
#if 1 //by gyu
#define CACHED_METADATA_BDROM 1
/*#define NON_CACHED_METADATA_BDROM 1*/

#define SB_READ_CACHE_MIN (0x10)
#define SB_READ_CACHE_MAX (0x2000)
#define SB_READ_CACHE_LIMIT (0x1000)
#define SB_READ_CACHE_MAGIC_NUM (0xdeadbeef)
#define BH_ACCESS_AFTER_CACHE_FULL (3)
#define BH_ACCESS_IS_UNREADABLE (2)
#define BH_ACCESS_IS_NON_MDATA (1)
typedef struct
{
        int m_policy;
        int sb_descriptor_validity[SB_READ_CACHE_MAX];
        int sb_descriptor_identity[SB_READ_CACHE_MAX];
        int sb_access_count[SB_READ_CACHE_MAX];
}
bdrom_cache_map;
typedef struct{
	void * sb_array[SB_READ_CACHE_MAX];
        void* p_latest_buffer_head;
        bdrom_cache_map m_dollarMap;
	int m_isCacheFull_01;
	int m_magicNumber;
	int m_isBdRom_01;
	int m_nPhysicalReads;
} bdrom_metadata_cache;
static bdrom_metadata_cache g_sb_read_cache;

#if defined (CACHED_METADATA_BDROM)
void udf_release_data(struct buffer_head *bh);
void bdrom_update_call_counts(int pos_012)
{
	static int thousand[10] = {0,0,0,0,0, 0,0,0,0,0};
	static int n_calls[10] = {0,0,0,0,0, 0,0,0,0,0};
	static int units[10] = {10000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
	int n;
	int start = 1;
	n_calls[pos_012]++;
	if( n_calls[pos_012] >= units[pos_012] )
	{
		thousand[pos_012]++;
		n_calls[pos_012] = 0;
		for( n = 0; n < 10; n++ )
		{
			if( (thousand[n] >= 1) || (n_calls[n] >= 1) )
			{
				if( start == 1 )
				{
					printk("\n\n\n###################################################\n");
					start = 0;
				}
				printk("Function[%d] called %d x %d + %d.\n", n, thousand[n], units[n], n_calls[n]);
			}
		}
		if( start == 0 )
		{
			printk("###################################################\n\n\n\n");
		}
	}
}

/*#define BDROM_CACHE_POLICY_SIMPLE __simpler_sb_bread*/
/*#define BDROM_CACHE_POLICY_SMART __intelligent_sb_bread*/
#define BDROM_CACHE_POLICY_INTELLIGENT __more_intelligent_sb_bread
inline int bdrom_get_policy(void)
{
	#ifdef BDROM_CACHE_POLICY_SIMPLE
		return 0;
	#endif
	#ifdef BDROM_CACHE_POLICY_SMART
		return 1;
	#endif
	#ifdef BDROM_CACHE_POLICY_INTELLIGENT
		return 2;
	#endif
}

inline int bdrom_metadata_cache_check_bh(void* p_obj)
{
	int block_nr;
	if( (((unsigned int)p_obj) & (unsigned int)0xFFFFfffc) == (unsigned int)0 )
	{
		return -1;
	}
	block_nr = ((struct buffer_head *)p_obj)->b_blocknr;
	if( (block_nr > SB_READ_CACHE_MAX) || (block_nr < 0) )
	{
		return -1;
	}
	return block_nr;
}
inline int bdrom_metadata_cache_is_mine(bdrom_metadata_cache* p_this, void* p_obj)
{
	int n;
	int block_nr;
	if( (block_nr = bdrom_metadata_cache_check_bh(p_obj)) == -1 )
	{
		return 0;
	}
	if( p_this->p_latest_buffer_head == p_obj )
	{
		return 1;
	}
	if( p_obj == p_this->sb_array[block_nr] )
	{
		return 1;
	}
	if( (((unsigned int)p_this->sb_array[block_nr]) & (unsigned int)0xFFFFfffc) == (unsigned int)0 )
	{
		return 0;
	}
	printk("bdrom_metadata_cache_is_mine::If you see this, it's a performance defect L1.\n");
	//printk("bdrom_metadata_cache_is_mine::If you see this, it's a performance defect L2.\n");
	for( n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++ )
	{
		if( p_this->sb_array[n] == p_obj ) return 1;
	}
	return 0;
}
int bdrom_metadata_cache_cleanup(bdrom_metadata_cache* p_this)
{
	int n;
	int nCachedSlots, nEmptySlots;
	struct buffer_head *bh = NULL;
	
	if( p_this->m_magicNumber != SB_READ_CACHE_MAGIC_NUM )
	{
		return 0;
	}
	invalidate_bh_lrus();
	nCachedSlots = nEmptySlots = 0;
	if( (p_this->m_isBdRom_01 == 1) && (p_this->m_magicNumber == SB_READ_CACHE_MAGIC_NUM) )
	{
		printk("bdrom_metadata_cache_cleanup::bdrom = true.\n");
		if( (p_this->m_isCacheFull_01 == 1) || (p_this->m_nPhysicalReads > 0) )
		{
			udf_debug("bdrom_metadata_cache_cleanup::entering the for loop...\n");
			for( n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++ )
			{
				if( bdrom_metadata_cache_check_bh(p_this->sb_array[n]) != -1 )
				{
					bh = p_this->sb_array[n];
					while (atomic_read(&bh->b_count))
					brelse(bh);
					p_this->sb_array[n] = (void*)0;
					nCachedSlots++;
				}
				else if( p_this->sb_array[n] == (void*)BH_ACCESS_IS_NON_MDATA )
				{
					nEmptySlots++;
				}
				else {
					nEmptySlots++;
				}
				if( p_this->m_dollarMap.m_policy == 2 )
				{

					if( bdrom_metadata_cache_check_bh(p_this->sb_array[n]) != -1 )
					{
						udf_debug("LBN[%d] - ACCESSED[%d] - validity[%d], ID[0x%x] - CACHED!!\n",
							n, p_this->m_dollarMap.sb_access_count[n],
							p_this->m_dollarMap.sb_descriptor_validity[n],
							p_this->m_dollarMap.sb_descriptor_identity[n]
						);
					}
					else if( p_this->m_dollarMap.sb_access_count[n] > 0 ){
						udf_debug("LBN[%d] - ACCESSED[%d] - validity[%d], ID[0x%x]\n",
							n, p_this->m_dollarMap.sb_access_count[n],
							p_this->m_dollarMap.sb_descriptor_validity[n],
							p_this->m_dollarMap.sb_descriptor_identity[n]
						);
					}
					p_this->m_dollarMap.sb_access_count[n] = 0;
					p_this->m_dollarMap.sb_descriptor_validity[n] = 0;
					p_this->m_dollarMap.sb_descriptor_identity[n] = 0;
				}
				p_this->sb_array[n] = (void*)0;
			}
			p_this->m_isCacheFull_01 = 0;
			p_this->m_nPhysicalReads = 0;
			//printk("\n\n############################################\n");
			udf_debug("Cached:Empty = %d:%d, ratio = %d\n",
				nCachedSlots, nEmptySlots, 100*nCachedSlots/(nCachedSlots + nEmptySlots)
			);
			//printk("############################################\n\n");
			return 1;
		}
	}
	printk("Skipping operations for non BD-ROM discs.\n");
	return 0;
}

int bdrom_cache_map_init(bdrom_cache_map* p_this)
{
	int n;
	p_this->m_policy = bdrom_get_policy();
	if( p_this->m_policy == 2 )
	{
		for( n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++ )
		{
			p_this->sb_access_count[n] = 0;
			p_this->sb_descriptor_validity[n] = 0;
			p_this->sb_descriptor_identity[n] = 0;
		}
		return 2;
	}
	return -1;
}

int bdrom_metadata_cache_on_init_udf(bdrom_metadata_cache *p_this, int is_bdrom_01)
{
	int n;

	if(p_this->m_magicNumber != SB_READ_CACHE_MAGIC_NUM){
		p_this->m_magicNumber  = SB_READ_CACHE_MAGIC_NUM;
		printk("bdrom_metadata_cache_on_init_udf::init. data structure.\n");
		for(n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++){
			p_this->sb_array[n] = (void*)0;
		}
		bdrom_cache_map_init(&p_this->m_dollarMap);
	}
	p_this->p_latest_buffer_head = (void*)0;
	p_this->m_isCacheFull_01 = 0;
	p_this->m_nPhysicalReads = 0;
	p_this->m_isBdRom_01 = is_bdrom_01;
	bdrom_metadata_cache_cleanup(p_this);
	return 0;
}

#define SB_READ_CACHE_TIMEOUT_IN_SECONDS 10
void *__simpler_sb_bread(struct super_block *sb, int lbn)
{
	int n, m;
	void *p_ret;

	if((g_sb_read_cache.m_isBdRom_01 == 1) && 
           (g_sb_read_cache.m_magicNumber == SB_READ_CACHE_MAGIC_NUM) &&
           (lbn < SB_READ_CACHE_MAX) && (lbn >= SB_READ_CACHE_MIN)){
		if(g_sb_read_cache.m_isCacheFull_01 == 0){
			for(n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++){
				udf_debug("_sb_bread() - reading block[%d]\n", n);
				g_sb_read_cache.sb_array[n] = sb_bread(sb, n);
				if (!(g_sb_read_cache.sb_array[n])){
					printk("_sb_bread() - Error reading block[%d]\n", n);
					for(m = 0; m < n; m++){
						udf_release_data(g_sb_read_cache.sb_array[m]);
					}
					for(m = n; m < SB_READ_CACHE_MAX; m++){
						g_sb_read_cache.sb_array[m] = (void*)0;
					}
					g_sb_read_cache.m_isCacheFull_01 = 1;
					return (void*)0;
				}
			}
			g_sb_read_cache.m_isCacheFull_01 = 1;
		}

                /*printk("_sb_bread::cache-hit-LBN(%d), buff-head(%d)\n", lbn, (int)g_sb_read_cache.sb_array[lbn]);*/
                if(g_sb_read_cache.sb_array[lbn] != (void*)0){
			return g_sb_read_cache.sb_array[lbn];
		}
	}
	p_ret = sb_bread(sb, lbn);
	/*printk("_sb_bread::cache-miss-LBN(%d), buff-head(%d)\n", lbn, (int)p_ret);*/
	return p_ret;
}

void* __intelligent_sb_bread(struct super_block *sb, int lbn)
{
	int n, m, n_next;
	void* p_ret;
	struct buffer_head* bh;
	if((g_sb_read_cache.m_isBdRom_01 == 1) &&
	   (g_sb_read_cache.m_magicNumber == SB_READ_CACHE_MAGIC_NUM) &&
	   (lbn < SB_READ_CACHE_MAX) && (lbn >= SB_READ_CACHE_MIN)){
		if(g_sb_read_cache.m_isCacheFull_01 == 0){
			struct timeval t1, t2;
			printk("_sb_bread() - reading block[%d]\n", lbn);
			do_gettimeofday(&t1);
			for(n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++){
				//printk("_sb_bread() - reading cache block[%d]\n", n);
				g_sb_read_cache.sb_array[n] = sb_bread(sb, n);
				do_gettimeofday(&t2);
                                if (!(g_sb_read_cache.sb_array[n])){
					printk("_sb_bread() - Error reading cache block %d\n", n);
					n_next = n +0x10;
					for(m = n; m < n_next; m++){
						g_sb_read_cache.sb_array[m] = (void*)0;
					}
					n = n_next;
					if ((t2.tv_sec - t1.tv_sec) > SB_READ_CACHE_TIMEOUT_IN_SECONDS){
						printk(KERN_ERR "_sb_bread() - Timed out reading cache block %d\n", lbn);
						for( m = n; m < SB_READ_CACHE_MAX; m++ ){
							g_sb_read_cache.sb_array[m] = (void*)0;
						}
						g_sb_read_cache.m_isCacheFull_01 = 1;
						return (void*)0;
					} else {
						printk("_sb_read::spent %d seconds while reading cache blocks...",
										(int) (t2.tv_sec - t1.tv_sec));
					}
				}
			}
			for( n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++ )
                                        {
                                                bh = (struct buffer_head *)g_sb_read_cache.sb_array[n];
                                                udf_debug("__intelligent_sb_bread::bh[%d]->b_blocknr = %llu\n", n, bh->b_blocknr);
                                        }
			g_sb_read_cache.m_isCacheFull_01 = 1;
		}
		/*printk("_sb_bread::cache-hit-LBN(%d), buff-head(%d)\n", lbn, (int)g_sb_read_cache.sb_array[lbn]);*/
		if(g_sb_read_cache.sb_array[lbn] != (void*)0){
			return g_sb_read_cache.sb_array[lbn];
		}
	}
	p_ret = sb_bread(sb, lbn);
	/*printk("_sb_bread::cache-miss-LBN(%d), buff-head(%d)\n", lbn, (int)p_ret);*/
	return p_ret;
}

int bdrom_metadata_cache_get_read_area_V01(
	bdrom_metadata_cache* p_this, int given_lbn, int* read_from_lbn, int* read_until_lbn ){
	if( (given_lbn < SB_READ_CACHE_MAX) && (given_lbn >= SB_READ_CACHE_MIN) )
	{
		if( given_lbn < SB_READ_CACHE_MAX*2/8 )
		{
			*read_from_lbn = SB_READ_CACHE_MIN;
			*read_until_lbn = SB_READ_CACHE_MAX*2/8 -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
		if( given_lbn < SB_READ_CACHE_MAX*3/8 )
		{
			*read_from_lbn = SB_READ_CACHE_MAX*2/8;
			*read_until_lbn = SB_READ_CACHE_MAX*3/8 -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
		if( given_lbn < SB_READ_CACHE_MAX*4/8 )
		{
			*read_from_lbn = SB_READ_CACHE_MAX*3/8;
			*read_until_lbn = SB_READ_CACHE_MAX*4/8 -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
		if( given_lbn < SB_READ_CACHE_MAX*5/8 )
		{
			*read_from_lbn = SB_READ_CACHE_MAX*4/8;
			*read_until_lbn = SB_READ_CACHE_MAX*5/8 -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
		if( given_lbn < SB_READ_CACHE_MAX*6/8 )
		{
			*read_from_lbn = SB_READ_CACHE_MAX*5/8;
			*read_until_lbn = SB_READ_CACHE_MAX*6/8 -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
		if( given_lbn < SB_READ_CACHE_MAX*7/8 )
		{
			*read_from_lbn = SB_READ_CACHE_MAX*6/8;
			*read_until_lbn = SB_READ_CACHE_MAX*7/8 -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
		if( given_lbn < SB_READ_CACHE_MAX )
		{
			*read_from_lbn = SB_READ_CACHE_MAX*7/8;
			*read_until_lbn = SB_READ_CACHE_MAX -1;
			return (*read_until_lbn)-(*read_from_lbn)+1;
		}
	}
	return 0;
}
int bdrom_metadata_cache_get_read_area_V02(
	bdrom_metadata_cache* p_this, int given_lbn, int* read_from_lbn, int* read_until_lbn)
{
	int start, until;
	if( (given_lbn < SB_READ_CACHE_MAX) && (given_lbn >= SB_READ_CACHE_MIN) )
	{
		start = given_lbn & 0xfffffff0;
		until = start + 255;
		if( until >= SB_READ_CACHE_MAX )
		{
			until = SB_READ_CACHE_MAX-1;
		}
		*read_from_lbn = start;
		*read_until_lbn = until;
		return until - start + 1;
	}
	return 0;
}

int bdrom_metadata_cache_get_read_area_V03(
	bdrom_metadata_cache* p_this, int given_lbn, int* read_from_lbn, int* read_until_lbn)
{
	int start, until;
	if( (given_lbn < SB_READ_CACHE_MAX) && (given_lbn >= SB_READ_CACHE_MIN) )
	{
		start = given_lbn & 0xfffffc00;
		until = start + 0x3ff;
		if( start == 0x00000000 )
		{
			start = SB_READ_CACHE_MIN;
		}
		if( until >= SB_READ_CACHE_MAX )
		{
			until = SB_READ_CACHE_MAX-1;
		}
		*read_from_lbn = start;
		*read_until_lbn = until;
		return until - start + 1;
	}
	return 0;
}

int bdrom_metadata_cache_get_read_area(
	bdrom_metadata_cache* p_this, int given_lbn, int* read_from_lbn, int* read_until_lbn)
{
	return bdrom_metadata_cache_get_read_area_V03(
		p_this, given_lbn, read_from_lbn, read_until_lbn);
}

int bdrom_metadata_cache_update_reads(bdrom_metadata_cache* p_this, int n_lbs_read)
{
	p_this->m_nPhysicalReads += n_lbs_read;
	udf_debug("p_this->m_nPhysicalReads = %d\n", p_this->m_nPhysicalReads);
	if( p_this->m_nPhysicalReads == (SB_READ_CACHE_MAX-SB_READ_CACHE_MIN+1) )
	{
		p_this->m_isCacheFull_01 = 1;
		return 1;
	}
	return 0;
}
int udf_is_valid_tag(uint16_t ident)
{
	switch(ident)
	{
		case TAG_IDENT_FSD:
			return 1;
		case TAG_IDENT_FID:
			return 2;
		case TAG_IDENT_AED:
			return 3;
		case TAG_IDENT_IE:
			return 4;
		case TAG_IDENT_TE:
			return 5;
		case TAG_IDENT_FE:
			return 6;
		case TAG_IDENT_EAHD:
			return 7;
		case TAG_IDENT_USE:
			return 8;
		case TAG_IDENT_SBD:
			return 9;
		case TAG_IDENT_PIE:
			return 10;
		case TAG_IDENT_EFE:
			return 11;
		default:
			return 0;
	}
	return -1;
}
/*
 ** crc.c
 **
 ** PURPOSE
 **      Routines to generate, calculate, and test a 16-bit CRC.
 **
 ** DESCRIPTION
 **      The CRC code was devised by Don P. Mitchell of AT&T Bell Laboratories
 **      and Ned W. Rhodes of Software Systems Group. It has been published in
 **      "Design and Validation of Computer Protocols", Prentice Hall,
 **      Englewood Cliffs, NJ, 1991, Chapter 3, ISBN 0-13-539925-4.
 **
 **      Copyright is held by AT&T.
 **
 **      AT&T gives permission for the free use of the CRC source code.
 **
 ** CONTACTS
 **      E-mail regarding any portion of the Linux UDF file system should be
 **      directed to the development team mailing list (run by majordomo):
 **              linux_udf@hpesjro.fc.hp.com
 **
 ** COPYRIGHT
 **      This file is distributed under the terms of the GNU General Public
 **      License (GPL). Copies of the GPL can be obtained from:
 **              ftp://prep.ai.mit.edu/pub/gnu/GPL
 **      Each contributing author retains all rights to their own work.
 **/
 
#include "udfdecl.h"
 
static uint16_t crc_table[256] = {
        0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50a5U, 0x60c6U, 0x70e7U,
        0x8108U, 0x9129U, 0xa14aU, 0xb16bU, 0xc18cU, 0xd1adU, 0xe1ceU, 0xf1efU,
        0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52b5U, 0x4294U, 0x72f7U, 0x62d6U,
        0x9339U, 0x8318U, 0xb37bU, 0xa35aU, 0xd3bdU, 0xc39cU, 0xf3ffU, 0xe3deU,
        0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64e6U, 0x74c7U, 0x44a4U, 0x5485U,
        0xa56aU, 0xb54bU, 0x8528U, 0x9509U, 0xe5eeU, 0xf5cfU, 0xc5acU, 0xd58dU,
        0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76d7U, 0x66f6U, 0x5695U, 0x46b4U,
        0xb75bU, 0xa77aU, 0x9719U, 0x8738U, 0xf7dfU, 0xe7feU, 0xd79dU, 0xc7bcU,
        0x48c4U, 0x58e5U, 0x6886U, 0x78a7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
        0xc9ccU, 0xd9edU, 0xe98eU, 0xf9afU, 0x8948U, 0x9969U, 0xa90aU, 0xb92bU,
        0x5af5U, 0x4ad4U, 0x7ab7U, 0x6a96U, 0x1a71U, 0x0a50U, 0x3a33U, 0x2a12U,
        0xdbfdU, 0xcbdcU, 0xfbbfU, 0xeb9eU, 0x9b79U, 0x8b58U, 0xbb3bU, 0xab1aU,
        0x6ca6U, 0x7c87U, 0x4ce4U, 0x5cc5U, 0x2c22U, 0x3c03U, 0x0c60U, 0x1c41U,
        0xedaeU, 0xfd8fU, 0xcdecU, 0xddcdU, 0xad2aU, 0xbd0bU, 0x8d68U, 0x9d49U,
        0x7e97U, 0x6eb6U, 0x5ed5U, 0x4ef4U, 0x3e13U, 0x2e32U, 0x1e51U, 0x0e70U,
        0xff9fU, 0xefbeU, 0xdfddU, 0xcffcU, 0xbf1bU, 0xaf3aU, 0x9f59U, 0x8f78U,
        0x9188U, 0x81a9U, 0xb1caU, 0xa1ebU, 0xd10cU, 0xc12dU, 0xf14eU, 0xe16fU,
        0x1080U, 0x00a1U, 0x30c2U, 0x20e3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
        0x83b9U, 0x9398U, 0xa3fbU, 0xb3daU, 0xc33dU, 0xd31cU, 0xe37fU, 0xf35eU,
        0x02b1U, 0x1290U, 0x22f3U, 0x32d2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
        0xb5eaU, 0xa5cbU, 0x95a8U, 0x8589U, 0xf56eU, 0xe54fU, 0xd52cU, 0xc50dU,
        0x34e2U, 0x24c3U, 0x14a0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
        0xa7dbU, 0xb7faU, 0x8799U, 0x97b8U, 0xe75fU, 0xf77eU, 0xc71dU, 0xd73cU,
        0x26d3U, 0x36f2U, 0x0691U, 0x16b0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
        0xd94cU, 0xc96dU, 0xf90eU, 0xe92fU, 0x99c8U, 0x89e9U, 0xb98aU, 0xa9abU,
        0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18c0U, 0x08e1U, 0x3882U, 0x28a3U,
        0xcb7dU, 0xdb5cU, 0xeb3fU, 0xfb1eU, 0x8bf9U, 0x9bd8U, 0xabbbU, 0xbb9aU,
        0x4a75U, 0x5a54U, 0x6a37U, 0x7a16U, 0x0af1U, 0x1ad0U, 0x2ab3U, 0x3a92U,
        0xfd2eU, 0xed0fU, 0xdd6cU, 0xcd4dU, 0xbdaaU, 0xad8bU, 0x9de8U, 0x8dc9U,
        0x7c26U, 0x6c07U, 0x5c64U, 0x4c45U, 0x3ca2U, 0x2c83U, 0x1ce0U, 0x0cc1U,
        0xef1fU, 0xff3eU, 0xcf5dU, 0xdf7cU, 0xaf9bU, 0xbfbaU, 0x8fd9U, 0x9ff8U,
        0x6e17U, 0x7e36U, 0x4e55U, 0x5e74U, 0x2e93U, 0x3eb2U, 0x0ed1U, 0x1ef0U
};
 
uint16_t
udf_crc(uint8_t *data, uint32_t size, uint16_t crc)
{
        while (size--)
                crc = crc_table[(crc >> 8 ^ *(data++)) & 0xffU] ^ (crc << 8);
 
        return crc;
}
 
int udf_identify_descriptor(struct super_block *sb, struct buffer_head *bh, uint16_t *ident)
{
	struct tag *tag_p;
	register uint8_t checksum;
	register int i;
	int cs_correct = 1;
	int crc_crrect = 0;
	int version_crrect = 1;

	if (!bh)
	{
		printk("[udf_identify_descriptor] bh == NULL\n");
		return -1;
	}
	tag_p = (struct tag *)(bh->b_data);

	*ident = le16_to_cpu(tag_p->tagIdent);

	checksum = 0U;
	for (i = 0; i < 4; i++)
	{
		checksum += (uint8_t)(bh->b_data[i]);
	}
	for (i = 5; i < 16; i++)
	{
		checksum += (uint8_t)(bh->b_data[i]);
	}
	if (checksum != tag_p->tagChecksum)
	{
		/*printk(KERN_ERR "[udf_identify_descriptor] tag checksum failed block\n");*/
		cs_correct = 0; /*-2*/
	}

	/* Verify the tag version */
	if (le16_to_cpu(tag_p->descVersion) != 0x0002U &&
		le16_to_cpu(tag_p->descVersion) != 0x0003U)
	{
		/*printk("[udf_identify_descriptor] tag version 0x%04x != 0x0002 || 0x0003\n",
			  le16_to_cpu(tag_p->descVersion));*/
		version_crrect = 0;/*-3*/
	}

	/* Verify the descriptor CRC */
	if (le16_to_cpu(tag_p->descCRCLength) + sizeof(struct tag) > sb->s_blocksize ||
		le16_to_cpu(tag_p->descCRC) == udf_crc(bh->b_data + sizeof(struct tag),
			le16_to_cpu(tag_p->descCRCLength), 0))
	{
		crc_crrect = 1;
	}
	/*
	else{
			printk("[udf_identify_descriptor] Crc failure : crc = %d, crclen = %d\n",
			le16_to_cpu(tag_p->descCRC), le16_to_cpu(tag_p->descCRCLength));
	}*/
	if( (cs_correct == 1) && (version_crrect == 0) )
	{
		return -3;
	}
	if( (cs_correct == 0) && (version_crrect == 1) )
	{
		return -4;
	}
	if( (cs_correct == 1) && (version_crrect == 1) && (crc_crrect == 1) )
	{
		return 1;
	}
	if( (cs_correct == 0) && (version_crrect == 0) && (crc_crrect == 0) )
	{
		return 2;
	}
	return -4;
}

int udf_ok_2_cache(uint16_t ident, int validity)
{
	if( (ident == 0x0000) && (validity == -3) )
	{
		return 0;
	}
	if( (ident == 0x504d) && (validity == -4) )
	{
		return 0;
	}
	return 1;
}

int bdrom_metadata_cache_is_ok_2_cache_more_blks(bdrom_metadata_cache* p_this)
{
	if( p_this->m_isCacheFull_01 == 1 )
	{
		udf_debug("_sb_bread() - cache full(1).\n");
		return 0;
	}
	if( p_this->m_nPhysicalReads >= SB_READ_CACHE_LIMIT )
	{
		udf_debug("_sb_bread() - cache full(2).\n");
		return 0;
	}
	return 1;
}

void udf_release_data_brcm(struct buffer_head *bh) //by gyu
{
       if (bh)
               brelse(bh);
}

void bdrom_metadata_cache_clean_preload_cache(bdrom_metadata_cache* p_this)
{
	int n;
	for( n = SB_READ_CACHE_MIN; n < SB_READ_CACHE_MAX; n++ ){
		if( bdrom_metadata_cache_check_bh(p_this->sb_array[n]) != -1 )
			udf_release_data_brcm(p_this->sb_array[n]);
		if( p_this->m_dollarMap.m_policy == 2 ){
			p_this->m_dollarMap.sb_access_count[n] = 0;
			p_this->m_dollarMap.sb_descriptor_validity[n] = 0;
			p_this->m_dollarMap.sb_descriptor_identity[n] = 0;
		}
                p_this->sb_array[n] = (void*)BH_ACCESS_AFTER_CACHE_FULL;
	}
        p_this->m_isCacheFull_01 = 1;
        p_this->m_nPhysicalReads = 0;
}

void bdrom_metadata_cache_halt_caching(bdrom_metadata_cache* p_this)
{
	if( p_this->m_isCacheFull_01 == 1 ){
		printk("bdrom_metadata_cache_halt_caching::abort...\n");
                return;
	}
        bdrom_metadata_cache_clean_preload_cache(p_this);
}

void* __more_intelligent_sb_bread(struct super_block *sb, int lbn)
{
	uint16_t ident;
	int n, m, n_next;
	int n_lbs_2_read, read_from, read_until, n_lbs_read = 0;
	void* p_ret;
	int ok_2_cache;
	if( (g_sb_read_cache.m_isBdRom_01 == 1) && (g_sb_read_cache.m_magicNumber == SB_READ_CACHE_MAGIC_NUM) )
	{
		if( (lbn < SB_READ_CACHE_MAX) && (lbn >= SB_READ_CACHE_MIN) )
		{
			if( g_sb_read_cache.m_isCacheFull_01 == 0 )
			{
				struct timeval t1, t2;
				if( bdrom_metadata_cache_check_bh(g_sb_read_cache.sb_array[lbn]) != -1 )
				{
					/*printk("M _sb_bread() - reading block [%d] from MEM\n", lbn);*/
					g_sb_read_cache.m_dollarMap.sb_access_count[lbn]++;
					g_sb_read_cache.p_latest_buffer_head = g_sb_read_cache.sb_array[lbn];
					return g_sb_read_cache.sb_array[lbn];
				}
				n_lbs_2_read = bdrom_metadata_cache_get_read_area(
					&g_sb_read_cache, lbn, &read_from, &read_until
				);
				if( n_lbs_2_read == 0 )
				{
					udf_debug("_sb_bread::error handling At-LBN(%d)\n", lbn);
					p_ret = sb_bread(sb, lbn);
					g_sb_read_cache.m_dollarMap.sb_access_count[lbn]++;
					return p_ret;
				}

				if( bdrom_metadata_cache_is_ok_2_cache_more_blks(&g_sb_read_cache) == 0 )
				{
					udf_debug("_sb_bread::cache-full At-LBN(%d)\n", lbn);
					g_sb_read_cache.m_isCacheFull_01 = 1;
					p_ret = sb_bread(sb, lbn);
					g_sb_read_cache.sb_array[lbn] = (void*)BH_ACCESS_AFTER_CACHE_FULL;
					/*Means that the corresponding sector was accessed after cache full*/
					g_sb_read_cache.m_dollarMap.sb_access_count[lbn]++;
					return p_ret;
				}

				udf_debug("###### Batch reading (%d ~ %d) ########\n", read_from, read_until);
				do_gettimeofday(&t1);
				
				/*read ahead all blocks */
				for( n = read_from; n <= read_until; n++ )
					sb_breadahead(sb, n );

				for( n = read_from; n <= read_until; n++ )
				{
					if( g_sb_read_cache.sb_array[n] == (void*)0 )
					{
						/*printk("B _sb_bread() - caching LBN[%d]\n", n);*/
						g_sb_read_cache.sb_array[n] = sb_bread(sb, n);
						do_gettimeofday(&t2);
                                                if (!(g_sb_read_cache.sb_array[n]) )
                                                {
                                                	if ( (t2.tv_sec - t1.tv_sec) > SB_READ_CACHE_TIMEOUT_IN_SECONDS )
                                                        {
                                                        	bdrom_metadata_cache_halt_caching(&g_sb_read_cache);
                                                                if( n == lbn )
                                                                	return (void*)0;
                                                                //else if( lbn == non cached or full or read error...)
                                                                p_ret = sb_bread(sb, lbn);
                                                                g_sb_read_cache.m_dollarMap.sb_access_count[lbn]++;
                                                                return p_ret;
                                                         }
                                                         n_next = ((n + 0x10) & ~0xF) -1;
                                                         for( m = n; m <= n_next; m++ ){
                                                         	g_sb_read_cache.sb_array[m] = (void*)BH_ACCESS_IS_UNREADABLE;
								// Means scratchy blocks, skip them
							 }
							 n = n_next;
							 continue;
                                                 }
						g_sb_read_cache.m_dollarMap.sb_descriptor_validity[n] =
                                                        udf_identify_descriptor(sb, (struct buffer_head*)g_sb_read_cache.sb_array[n], &ident);
                                                g_sb_read_cache.m_dollarMap.sb_descriptor_identity[n] = ident;
                                                ok_2_cache = udf_ok_2_cache(
                                                        g_sb_read_cache.m_dollarMap.sb_descriptor_identity[n],
                                                        g_sb_read_cache.m_dollarMap.sb_descriptor_validity[n]
                                                );
                                                if( ok_2_cache == 0 && (lbn != n))
                                                {
                                                        udf_release_data_brcm(g_sb_read_cache.sb_array[n]);
                                                        g_sb_read_cache.sb_array[n] = (void*)BH_ACCESS_IS_NON_MDATA;
                                                        /*Means that the corresponding sector is not a useful i-node block*/
                                                }
						if( g_sb_read_cache.sb_array[n] != (void*)BH_ACCESS_IS_NON_MDATA )
                                                {
                                                        n_lbs_read++;
                                                }
						 
					}
				}
				bdrom_metadata_cache_update_reads(&g_sb_read_cache, n_lbs_read);
			}
			if( bdrom_metadata_cache_check_bh(g_sb_read_cache.sb_array[lbn]) != -1 )
			{
				udf_debug("M _sb_bread() - reading block [%d] from MEM\n", lbn);
				g_sb_read_cache.m_dollarMap.sb_access_count[lbn]++;
				g_sb_read_cache.p_latest_buffer_head = g_sb_read_cache.sb_array[lbn];
				return g_sb_read_cache.sb_array[lbn];
			}
		}
	}
	//printk("_sb_bread::cache-miss-LBN(%d)\n", lbn);
	p_ret = sb_bread(sb, lbn);
	return p_ret;
}

void* _sb_bread(struct super_block *sb, int lbn)
{
	int policy = bdrom_get_policy();
	switch(policy)
	{
		case 0:
			return __simpler_sb_bread(sb, lbn);
		case 1:
			return __intelligent_sb_bread(sb, lbn);
		case 2:
			return __more_intelligent_sb_bread(sb, lbn);
		default:
			printk("_sb_bread::unexpected corner case-1 reached.\n");
			return __simpler_sb_bread(sb, lbn);
	}
	//printk("_sb_bread::unexpected corner case-2 reached.\n");
	return __simpler_sb_bread(sb, lbn);
}

void udf_release_data(struct buffer_head *bh)
{
	if((g_sb_read_cache.m_isBdRom_01 == 1) &&
	   (g_sb_read_cache.m_magicNumber == SB_READ_CACHE_MAGIC_NUM)){
		if(bdrom_metadata_cache_is_mine(&g_sb_read_cache, (void*)bh) == 1){
			/*printk("udf_release_data::cached-buff-head(%d)\n", (int)bh);*/
			return;
		}
	}
	brelse(bh);
	return;
}
#endif

#if defined (NON_CACHED_METADATA_BDROM)
int bdrom_metadata_cache_on_init_udf(bdrom_metadata_cache *p_this, int is_bdrom_01)
{
	return 0;
}

int bdrom_metadata_cache_cleanup(bdrom_metadata_cache *p_this)
{
	return 0;
}

void *_sb_bread(struct super_block *sb, int lbn)
{
	return sb_bread(sb, lbn);
}

void udf_release_data(struct buffer_head *bh)
{
	brelse(bh);
}
#endif
#endif //by gyu

struct logicalVolIntegrityDescImpUse *udf_sb_lvidiu(struct udf_sb_info *sbi)
{
	struct logicalVolIntegrityDesc *lvid =
		(struct logicalVolIntegrityDesc *)sbi->s_lvid_bh->b_data;
	__u32 number_of_partitions = le32_to_cpu(lvid->numOfPartitions);
	__u32 offset = number_of_partitions * 2 *
				sizeof(uint32_t)/sizeof(uint8_t);
	return (struct logicalVolIntegrityDescImpUse *)&(lvid->impUse[offset]);
}

/* UDF filesystem type */
static struct dentry *udf_mount(struct file_system_type *fs_type,
		      int flags, const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, udf_fill_super);
}

static struct file_system_type udf_fstype = {
	.owner		= THIS_MODULE,
	.name		= "udf",
	.mount		= udf_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};

static struct kmem_cache *udf_inode_cachep;

static struct inode *udf_alloc_inode(struct super_block *sb)
{
	struct udf_inode_info *ei;
	ei = kmem_cache_alloc(udf_inode_cachep, GFP_KERNEL);
	if (!ei)
		return NULL;

	ei->i_unique = 0;
	ei->i_lenExtents = 0;
	ei->i_next_alloc_block = 0;
	ei->i_next_alloc_goal = 0;
	ei->i_strat4096 = 0;
	init_rwsem(&ei->i_data_sem);
	atomic_set(&ei->extent_desc_cache.ref_count, 0);
	memset(&ei->recent_access, 0, sizeof(udf_extent_cache));
	ei->recent_access.udf_pos.block_id = -1;

	return &ei->vfs_inode;
}

static void udf_i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);
	INIT_LIST_HEAD(&inode->i_dentry);
	kmem_cache_free(udf_inode_cachep, UDF_I(inode));
}

static void udf_destroy_inode(struct inode *inode)
{
	call_rcu(&inode->i_rcu, udf_i_callback);
}

static void init_once(void *foo)
{
	struct udf_inode_info *ei = (struct udf_inode_info *)foo;

	ei->i_ext.i_data = NULL;
	inode_init_once(&ei->vfs_inode);
}

static int init_inodecache(void)
{
	udf_inode_cachep = kmem_cache_create("udf_inode_cache",
					     sizeof(struct udf_inode_info),
					     0, (SLAB_RECLAIM_ACCOUNT |
						 SLAB_MEM_SPREAD),
					     init_once);
	if (!udf_inode_cachep)
		return -ENOMEM;
	return 0;
}

static void destroy_inodecache(void)
{
	kmem_cache_destroy(udf_inode_cachep);
}

/* Superblock operations */
static const struct super_operations udf_sb_ops = {
	.alloc_inode	= udf_alloc_inode,
	.destroy_inode	= udf_destroy_inode,
	.write_inode	= udf_write_inode,
	.evict_inode	= udf_evict_inode,
	.put_super	= udf_put_super,
	.sync_fs	= udf_sync_fs,
	.statfs		= udf_statfs,
	.remount_fs	= udf_remount_fs,
	.show_options	= udf_show_options,
};

struct udf_options {
	unsigned char novrs;
	unsigned int blocksize;
	unsigned int session;
	unsigned int lastblock;
	unsigned int anchor;
	unsigned int volume;
	unsigned short partition;
	unsigned int fileset;
	unsigned int rootdir;
	unsigned int flags;
	mode_t umask;
	gid_t gid;
	uid_t uid;
	mode_t fmode;
	mode_t dmode;
	struct nls_table *nls_map;
	unsigned int bdrom; //by gyu
};

static int __init init_udf_fs(void)
{
	int err;

	err = init_inodecache();
	if (err)
		goto out1;
	err = register_filesystem(&udf_fstype);
	if (err)
		goto out;

	return 0;

out:
	destroy_inodecache();

out1:
	return err;
}

static void __exit exit_udf_fs(void)
{
	unregister_filesystem(&udf_fstype);
	destroy_inodecache();
}

module_init(init_udf_fs)
module_exit(exit_udf_fs)

static int udf_sb_alloc_partition_maps(struct super_block *sb, u32 count)
{
	struct udf_sb_info *sbi = UDF_SB(sb);

	sbi->s_partmaps = kcalloc(count, sizeof(struct udf_part_map),
				  GFP_KERNEL);
	if (!sbi->s_partmaps) {
		udf_error(sb, __func__,
			  "Unable to allocate space for %d partition maps",
			  count);
		sbi->s_partitions = 0;
		return -ENOMEM;
	}

	sbi->s_partitions = count;
	return 0;
}

static int udf_show_options(struct seq_file *seq, struct vfsmount *mnt)
{
	struct super_block *sb = mnt->mnt_sb;
	struct udf_sb_info *sbi = UDF_SB(sb);

	if (!UDF_QUERY_FLAG(sb, UDF_FLAG_STRICT))
		seq_puts(seq, ",nostrict");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_BLOCKSIZE_SET))
		seq_printf(seq, ",bs=%lu", sb->s_blocksize);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UNHIDE))
		seq_puts(seq, ",unhide");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UNDELETE))
		seq_puts(seq, ",undelete");
	if (!UDF_QUERY_FLAG(sb, UDF_FLAG_USE_AD_IN_ICB))
		seq_puts(seq, ",noadinicb");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_USE_SHORT_AD))
		seq_puts(seq, ",shortad");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UID_FORGET))
		seq_puts(seq, ",uid=forget");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UID_IGNORE))
		seq_puts(seq, ",uid=ignore");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_GID_FORGET))
		seq_puts(seq, ",gid=forget");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_GID_IGNORE))
		seq_puts(seq, ",gid=ignore");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UID_SET))
		seq_printf(seq, ",uid=%u", sbi->s_uid);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_GID_SET))
		seq_printf(seq, ",gid=%u", sbi->s_gid);
	if (sbi->s_umask != 0)
		seq_printf(seq, ",umask=%o", sbi->s_umask);
	if (sbi->s_fmode != UDF_INVALID_MODE)
		seq_printf(seq, ",mode=%o", sbi->s_fmode);
	if (sbi->s_dmode != UDF_INVALID_MODE)
		seq_printf(seq, ",dmode=%o", sbi->s_dmode);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_SESSION_SET))
		seq_printf(seq, ",session=%u", sbi->s_session);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_LASTBLOCK_SET))
		seq_printf(seq, ",lastblock=%u", sbi->s_last_block);
	if (sbi->s_anchor != 0)
		seq_printf(seq, ",anchor=%u", sbi->s_anchor);
	/*
	 * volume, partition, fileset and rootdir seem to be ignored
	 * currently
	 */
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8))
		seq_puts(seq, ",utf8");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP) && sbi->s_nls_map)
		seq_printf(seq, ",iocharset=%s", sbi->s_nls_map->charset);

	return 0;
}

/*
 * udf_parse_options
 *
 * PURPOSE
 *	Parse mount options.
 *
 * DESCRIPTION
 *	The following mount options are supported:
 *
 *	gid=		Set the default group.
 *	umask=		Set the default umask.
 *	mode=		Set the default file permissions.
 *	dmode=		Set the default directory permissions.
 *	uid=		Set the default user.
 *	bs=		Set the block size.
 *	unhide		Show otherwise hidden files.
 *	undelete	Show deleted files in lists.
 *	adinicb		Embed data in the inode (default)
 *	noadinicb	Don't embed data in the inode
 *	shortad		Use short ad's
 *	longad		Use long ad's (default)
 *	nostrict	Unset strict conformance
 *	iocharset=	Set the NLS character set
 *
 *	The remaining are for debugging and disaster recovery:
 *
 *	novrs		Skip volume sequence recognition
 *
 *	The following expect a offset from 0.
 *
 *	session=	Set the CDROM session (default= last session)
 *	anchor=		Override standard anchor location. (default= 256)
 *	volume=		Override the VolumeDesc location. (unused)
 *	partition=	Override the PartitionDesc location. (unused)
 *	lastblock=	Set the last block of the filesystem/
 *
 *	The following expect a offset from the partition root.
 *
 *	fileset=	Override the fileset block location. (unused)
 *	rootdir=	Override the root directory location. (unused)
 *		WARNING: overriding the rootdir to a non-directory may
 *		yield highly unpredictable results.
 *
 * PRE-CONDITIONS
 *	options		Pointer to mount options string.
 *	uopts		Pointer to mount options variable.
 *
 * POST-CONDITIONS
 *	<return>	1	Mount options parsed okay.
 *	<return>	0	Error parsing mount options.
 *
 * HISTORY
 *	July 1, 1997 - Andrew E. Mileski
 *	Written, tested, and released.
 */

enum {
	Opt_novrs, Opt_nostrict, Opt_bs, Opt_unhide, Opt_undelete,
	Opt_noadinicb, Opt_adinicb, Opt_shortad, Opt_longad,
	Opt_gid, Opt_uid, Opt_umask, Opt_session, Opt_lastblock,
	Opt_anchor, Opt_volume, Opt_partition, Opt_fileset,
	Opt_rootdir, Opt_utf8, Opt_iocharset,
	Opt_err, Opt_uforget, Opt_uignore, Opt_gforget, Opt_gignore,
	Opt_fmode, Opt_dmode, Opt_bdrom //by gyu
};

static const match_table_t tokens = {
	{Opt_novrs,	"novrs"},
	{Opt_nostrict,	"nostrict"},
	{Opt_bs,	"bs=%u"},
	{Opt_unhide,	"unhide"},
	{Opt_undelete,	"undelete"},
	{Opt_noadinicb,	"noadinicb"},
	{Opt_adinicb,	"adinicb"},
	{Opt_shortad,	"shortad"},
	{Opt_longad,	"longad"},
	{Opt_uforget,	"uid=forget"},
	{Opt_uignore,	"uid=ignore"},
	{Opt_gforget,	"gid=forget"},
	{Opt_gignore,	"gid=ignore"},
	{Opt_gid,	"gid=%u"},
	{Opt_uid,	"uid=%u"},
	{Opt_umask,	"umask=%o"},
	{Opt_session,	"session=%u"},
	{Opt_lastblock,	"lastblock=%u"},
	{Opt_anchor,	"anchor=%u"},
	{Opt_volume,	"volume=%u"},
	{Opt_partition,	"partition=%u"},
	{Opt_fileset,	"fileset=%u"},
	{Opt_rootdir,	"rootdir=%u"},
	{Opt_utf8,	"utf8"},
	{Opt_iocharset,	"iocharset=%s"},
	{Opt_fmode,     "mode=%o"},
	{Opt_dmode,     "dmode=%o"},
	{Opt_bdrom, 	"bdrom"}, //by gyu
	{Opt_err,	NULL}
};

static int udf_parse_options(char *options, struct udf_options *uopt,
			     bool remount)
{
	char *p;
	int option;

	uopt->novrs = 0;
	uopt->partition = 0xFFFF;
	uopt->session = 0xFFFFFFFF;
	uopt->lastblock = 0;
	uopt->anchor = 0;
	uopt->volume = 0xFFFFFFFF;
	uopt->rootdir = 0xFFFFFFFF;
	uopt->fileset = 0xFFFFFFFF;
	uopt->nls_map = NULL;

	if (!options)
		return 1;

	while ((p = strsep(&options, ",")) != NULL) {
		substring_t args[MAX_OPT_ARGS];
		int token;
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_novrs:
			uopt->novrs = 1;
			break;
		case Opt_bs:
			if (match_int(&args[0], &option))
				return 0;
			uopt->blocksize = option;
			uopt->flags |= (1 << UDF_FLAG_BLOCKSIZE_SET);
			break;
		case Opt_unhide:
			uopt->flags |= (1 << UDF_FLAG_UNHIDE);
			break;
		case Opt_undelete:
			uopt->flags |= (1 << UDF_FLAG_UNDELETE);
			break;
		case Opt_noadinicb:
			uopt->flags &= ~(1 << UDF_FLAG_USE_AD_IN_ICB);
			break;
		case Opt_adinicb:
			uopt->flags |= (1 << UDF_FLAG_USE_AD_IN_ICB);
			break;
		case Opt_shortad:
			uopt->flags |= (1 << UDF_FLAG_USE_SHORT_AD);
			break;
		case Opt_longad:
			uopt->flags &= ~(1 << UDF_FLAG_USE_SHORT_AD);
			break;
		case Opt_gid:
			if (match_int(args, &option))
				return 0;
			uopt->gid = option;
			uopt->flags |= (1 << UDF_FLAG_GID_SET);
			break;
		case Opt_uid:
			if (match_int(args, &option))
				return 0;
			uopt->uid = option;
			uopt->flags |= (1 << UDF_FLAG_UID_SET);
			break;
		case Opt_umask:
			if (match_octal(args, &option))
				return 0;
			uopt->umask = option;
			break;
		case Opt_nostrict:
			uopt->flags &= ~(1 << UDF_FLAG_STRICT);
			break;
		case Opt_session:
			if (match_int(args, &option))
				return 0;
			uopt->session = option;
			if (!remount)
				uopt->flags |= (1 << UDF_FLAG_SESSION_SET);
			break;
		case Opt_lastblock:
			if (match_int(args, &option))
				return 0;
			uopt->lastblock = option;
			if (!remount)
				uopt->flags |= (1 << UDF_FLAG_LASTBLOCK_SET);
			break;
		case Opt_anchor:
			if (match_int(args, &option))
				return 0;
			uopt->anchor = option;
			break;
		case Opt_volume:
			if (match_int(args, &option))
				return 0;
			uopt->volume = option;
			break;
		case Opt_partition:
			if (match_int(args, &option))
				return 0;
			uopt->partition = option;
			break;
		case Opt_fileset:
			if (match_int(args, &option))
				return 0;
			uopt->fileset = option;
			break;
		case Opt_rootdir:
			if (match_int(args, &option))
				return 0;
			uopt->rootdir = option;
			break;
		case Opt_utf8:
			uopt->flags |= (1 << UDF_FLAG_UTF8);
			break;
#ifdef CONFIG_UDF_NLS
		case Opt_iocharset:
			uopt->nls_map = load_nls(args[0].from);
			uopt->flags |= (1 << UDF_FLAG_NLS_MAP);
			break;
#endif
		case Opt_uignore:
			uopt->flags |= (1 << UDF_FLAG_UID_IGNORE);
			break;
		case Opt_uforget:
			uopt->flags |= (1 << UDF_FLAG_UID_FORGET);
			break;
		case Opt_gignore:
			uopt->flags |= (1 << UDF_FLAG_GID_IGNORE);
			break;
		case Opt_gforget:
			uopt->flags |= (1 << UDF_FLAG_GID_FORGET);
			break;
		case Opt_fmode:
			if (match_octal(args, &option))
				return 0;
			uopt->fmode = option & 0777;
			break;
		case Opt_dmode:
			if (match_octal(args, &option))
				return 0;
			uopt->dmode = option & 0777;
			break;
#if 1 //by gyu
		case Opt_bdrom:
			printk("bdrom option identified.\n");
			uopt->bdrom = 1;
			break;
#endif //by gyu
		default:
			printk(KERN_ERR "udf: bad mount option \"%s\" "
			       "or missing value\n", p);
			return 0;
		}
	}
	return 1;
}

static int udf_remount_fs(struct super_block *sb, int *flags, char *options)
{
	struct udf_options uopt;
	struct udf_sb_info *sbi = UDF_SB(sb);
	int error = 0;

	uopt.flags = sbi->s_flags;
	uopt.uid   = sbi->s_uid;
	uopt.gid   = sbi->s_gid;
	uopt.umask = sbi->s_umask;
	uopt.fmode = sbi->s_fmode;
	uopt.dmode = sbi->s_dmode;

	if (!udf_parse_options(options, &uopt, true))
		return -EINVAL;

	write_lock(&sbi->s_cred_lock);
	sbi->s_flags = uopt.flags;
	sbi->s_uid   = uopt.uid;
	sbi->s_gid   = uopt.gid;
	sbi->s_umask = uopt.umask;
	sbi->s_fmode = uopt.fmode;
	sbi->s_dmode = uopt.dmode;
	write_unlock(&sbi->s_cred_lock);

	if (sbi->s_lvid_bh) {
		int write_rev = le16_to_cpu(udf_sb_lvidiu(sbi)->minUDFWriteRev);
		if (write_rev > UDF_MAX_WRITE_VERSION)
			*flags |= MS_RDONLY;
	}

	if ((*flags & MS_RDONLY) == (sb->s_flags & MS_RDONLY))
		goto out_unlock;

	if (*flags & MS_RDONLY)
		udf_close_lvid(sb);
	else
		udf_open_lvid(sb);

out_unlock:
	return error;
}

/* Check Volume Structure Descriptors (ECMA 167 2/9.1) */
/* We also check any "CD-ROM Volume Descriptor Set" (ECMA 167 2/8.3.1) */
static loff_t udf_check_vsd(struct super_block *sb)
{
	struct volStructDesc *vsd = NULL;
	loff_t sector = 32768;
	int sectorsize;
	struct buffer_head *bh = NULL;
	int nsr02 = 0;
	int nsr03 = 0;
	struct udf_sb_info *sbi;

	sbi = UDF_SB(sb);
	if (sb->s_blocksize < sizeof(struct volStructDesc))
		sectorsize = sizeof(struct volStructDesc);
	else
		sectorsize = sb->s_blocksize;

	sector += (sbi->s_session << sb->s_blocksize_bits);

	udf_debug("Starting at sector %u (%ld byte sectors)\n",
		  (unsigned int)(sector >> sb->s_blocksize_bits),
		  sb->s_blocksize);
	/* Process the sequence (if applicable) */
	for (; !nsr02 && !nsr03; sector += sectorsize) {
		/* Read a block */
		bh = udf_tread(sb, sector >> sb->s_blocksize_bits);
		if (!bh)
			break;

		/* Look for ISO  descriptors */
		vsd = (struct volStructDesc *)(bh->b_data +
					      (sector & (sb->s_blocksize - 1)));

		if (vsd->stdIdent[0] == 0) {
			udf_release_data(bh);
			break;
		} else if (!strncmp(vsd->stdIdent, VSD_STD_ID_CD001,
				    VSD_STD_ID_LEN)) {
			switch (vsd->structType) {
			case 0:
				udf_debug("ISO9660 Boot Record found\n");
				break;
			case 1:
				udf_debug("ISO9660 Primary Volume Descriptor "
					  "found\n");
				break;
			case 2:
				udf_debug("ISO9660 Supplementary Volume "
					  "Descriptor found\n");
				break;
			case 3:
				udf_debug("ISO9660 Volume Partition Descriptor "
					  "found\n");
				break;
			case 255:
				udf_debug("ISO9660 Volume Descriptor Set "
					  "Terminator found\n");
				break;
			default:
				udf_debug("ISO9660 VRS (%u) found\n",
					  vsd->structType);
				break;
			}
		} else if (!strncmp(vsd->stdIdent, VSD_STD_ID_BEA01,
				    VSD_STD_ID_LEN))
			; /* nothing */
		else if (!strncmp(vsd->stdIdent, VSD_STD_ID_TEA01,
				    VSD_STD_ID_LEN)) {
			udf_release_data(bh);
			break;
		} else if (!strncmp(vsd->stdIdent, VSD_STD_ID_NSR02,
				    VSD_STD_ID_LEN))
			nsr02 = sector;
		else if (!strncmp(vsd->stdIdent, VSD_STD_ID_NSR03,
				    VSD_STD_ID_LEN))
			nsr03 = sector;
		udf_release_data(bh);
	}

	if (nsr03)
		return nsr03;
	else if (nsr02)
		return nsr02;
	else if (sector - (sbi->s_session << sb->s_blocksize_bits) == 32768)
		return -1;
	else
		return 0;
}

static int udf_find_fileset(struct super_block *sb,
			    struct kernel_lb_addr *fileset,
			    struct kernel_lb_addr *root)
{
	struct buffer_head *bh = NULL;
	long lastblock;
	uint16_t ident;
	struct udf_sb_info *sbi;

	if (fileset->logicalBlockNum != 0xFFFFFFFF ||
	    fileset->partitionReferenceNum != 0xFFFF) {
		bh = udf_read_ptagged(sb, fileset, 0, &ident);

		if (!bh) {
			return 1;
		} else if (ident != TAG_IDENT_FSD) {
			udf_release_data(bh);
			return 1;
		}

	}

	sbi = UDF_SB(sb);
	if (!bh) {
		/* Search backwards through the partitions */
		struct kernel_lb_addr newfileset;

/* --> cvg: FIXME - is it reasonable? */
		return 1;

		for (newfileset.partitionReferenceNum = sbi->s_partitions - 1;
		     (newfileset.partitionReferenceNum != 0xFFFF &&
		      fileset->logicalBlockNum == 0xFFFFFFFF &&
		      fileset->partitionReferenceNum == 0xFFFF);
		     newfileset.partitionReferenceNum--) {
			lastblock = sbi->s_partmaps
					[newfileset.partitionReferenceNum]
						.s_partition_len;
			newfileset.logicalBlockNum = 0;

			do {
				bh = udf_read_ptagged(sb, &newfileset, 0,
						      &ident);
				if (!bh) {
					newfileset.logicalBlockNum++;
					continue;
				}

				switch (ident) {
				case TAG_IDENT_SBD:
				{
					struct spaceBitmapDesc *sp;
					sp = (struct spaceBitmapDesc *)
								bh->b_data;
					newfileset.logicalBlockNum += 1 +
						((le32_to_cpu(sp->numOfBytes) +
						  sizeof(struct spaceBitmapDesc)
						  - 1) >> sb->s_blocksize_bits);
					udf_release_data(bh);
					break;
				}
				case TAG_IDENT_FSD:
					*fileset = newfileset;
					break;
				default:
					newfileset.logicalBlockNum++;
					udf_release_data(bh);
					bh = NULL;
					break;
				}
			} while (newfileset.logicalBlockNum < lastblock &&
				 fileset->logicalBlockNum == 0xFFFFFFFF &&
				 fileset->partitionReferenceNum == 0xFFFF);
		}
	}

	if ((fileset->logicalBlockNum != 0xFFFFFFFF ||
	     fileset->partitionReferenceNum != 0xFFFF) && bh) {
		udf_debug("Fileset at block=%d, partition=%d\n",
			  fileset->logicalBlockNum,
			  fileset->partitionReferenceNum);

		sbi->s_partition = fileset->partitionReferenceNum;
		udf_load_fileset(sb, bh, root);
		udf_release_data(bh);
		return 0;
	}
	return 1;
}

static int udf_load_pvoldesc(struct super_block *sb, sector_t block)
{
	struct primaryVolDesc *pvoldesc;
	struct ustr *instr, *outstr;
	struct buffer_head *bh;
	uint16_t ident;
	int ret = 1;

	instr = kmalloc(sizeof(struct ustr), GFP_NOFS);
	if (!instr)
		return 1;

	outstr = kmalloc(sizeof(struct ustr), GFP_NOFS);
	if (!outstr)
		goto out1;

	bh = udf_read_tagged(sb, block, block, &ident);
	if (!bh)
		goto out2;

	BUG_ON(ident != TAG_IDENT_PVD);

	pvoldesc = (struct primaryVolDesc *)bh->b_data;

	if (udf_disk_stamp_to_time(&UDF_SB(sb)->s_record_time,
			      pvoldesc->recordingDateAndTime)) {
#ifdef UDFFS_DEBUG
		struct timestamp *ts = &pvoldesc->recordingDateAndTime;
		udf_debug("recording time %04u/%02u/%02u"
			  " %02u:%02u (%x)\n",
			  le16_to_cpu(ts->year), ts->month, ts->day, ts->hour,
			  ts->minute, le16_to_cpu(ts->typeAndTimezone));
#endif
	}

	if (!udf_build_ustr(instr, pvoldesc->volIdent, 32))
		if (udf_CS0toUTF8(outstr, instr)) {
			strncpy(UDF_SB(sb)->s_volume_ident, outstr->u_name,
				outstr->u_len > 31 ? 31 : outstr->u_len);
			udf_debug("volIdent[] = '%s'\n",
					UDF_SB(sb)->s_volume_ident);
		}

	if (!udf_build_ustr(instr, pvoldesc->volSetIdent, 128))
		if (udf_CS0toUTF8(outstr, instr))
			udf_debug("volSetIdent[] = '%s'\n", outstr->u_name);

	udf_release_data(bh);
	ret = 0;
out2:
	kfree(outstr);
out1:
	kfree(instr);
	return ret;
}

struct inode *udf_find_metadata_inode_efe(struct super_block *sb,
					u32 meta_file_loc, u32 partition_num)
{
	struct kernel_lb_addr addr;
	struct inode *metadata_fe;

	addr.logicalBlockNum = meta_file_loc;
	addr.partitionReferenceNum = partition_num;

	metadata_fe = udf_iget(sb, &addr);

	if (metadata_fe == NULL)
		udf_warning(sb,__func__, "metadata inode efe not found\n");
	else if (UDF_I(metadata_fe)->i_alloc_type != ICBTAG_FLAG_AD_SHORT) {
		udf_warning(sb,__func__, "metadata inode efe does not have short allocation descriptors!\n");
		iput(metadata_fe);
		metadata_fe = NULL;
	}

	return metadata_fe;
}


static int udf_load_metadata_files(struct super_block *sb, int partition)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct udf_part_map *map;
	struct udf_meta_data *mdata;
	struct kernel_lb_addr addr;
        int mdata_number_iscorrect=0;
        int i;

restart:
	map = &sbi->s_partmaps[partition];
	mdata = &map->s_type_specific.s_metadata;

	/* metadata address */
	udf_debug("Metadata file location: block = %d part = %d\n",
			   mdata->s_meta_file_loc, map->s_partition_num);

	mdata->s_metadata_fe = udf_find_metadata_inode_efe(sb,
		mdata->s_meta_file_loc, map->s_partition_num);

	if (mdata->s_metadata_fe == NULL) {
        if( !mdata_number_iscorrect){
                for (i = 0; i < sbi->s_partitions ; i++)
                        if(sbi->s_partmaps[i].s_partition_type == UDF_TYPE1_MAP15  || sbi->s_partmaps[i].s_partition_type == UDF_SPARABLE_MAP15)
                                break;
                udf_debug("Correct Metadata Partition number from: %d, to %d\n",sbi->s_partmaps[i].s_partition_num, i);
                sbi->s_partmaps[partition].s_partition_num = i;
                mdata_number_iscorrect=1;
                goto restart;
        }

	/* mirror file entry */
	udf_debug("Mirror metadata file location: block = %d part = %d\n",
		mdata->s_mirror_file_loc, mdata->s_mirror_file_loc);
	mdata->s_mirror_fe = udf_find_metadata_inode_efe(sb,
			mdata->s_mirror_file_loc, map->s_partition_num);
		if (mdata->s_mirror_fe == NULL) {
			udf_error(sb, __func__, "Both metadata and mirror metadata inode efe can not found\n");
			goto error_exit;
		}
	}

	/*
	 * bitmap file entry
	 * Note:
	 * Load only if bitmap file location differs from 0xFFFFFFFF (DCN-5102)
	*/
	if (mdata->s_bitmap_file_loc != 0xFFFFFFFF) {
		addr.logicalBlockNum = mdata->s_bitmap_file_loc;
		addr.partitionReferenceNum = map->s_partition_num;

		udf_debug("Bitmap file location: block = %d part = %d\n",
			addr.logicalBlockNum, addr.partitionReferenceNum);

		mdata->s_bitmap_fe = udf_iget(sb, &addr);

		if (mdata->s_bitmap_fe == NULL) {
			if (sb->s_flags & MS_RDONLY)
				udf_warning(sb, __func__, "bitmap inode efe "
					"not found but it's ok since the disc"
					" is mounted read-only");
			else {
				udf_error(sb, __func__, "bitmap inode efe not "
					"found and attempted read-write mount");
				goto error_exit;
			}
		}
	}

	udf_debug("udf_load_metadata_files Ok\n");

	return 0;

error_exit:
	return 1;
}

static void udf_load_fileset(struct super_block *sb, struct buffer_head *bh,
			     struct kernel_lb_addr *root)
{
	struct fileSetDesc *fset;

	fset = (struct fileSetDesc *)bh->b_data;

	*root = lelb_to_cpu(fset->rootDirectoryICB.extLocation);

	UDF_SB(sb)->s_serial_number = le16_to_cpu(fset->descTag.tagSerialNum);

	udf_debug("Rootdir at block=%d, partition=%d\n",
		  root->logicalBlockNum, root->partitionReferenceNum);
}

int udf_compute_nr_groups(struct super_block *sb, u32 partition)
{
	struct udf_part_map *map = &UDF_SB(sb)->s_partmaps[partition];
	return DIV_ROUND_UP(map->s_partition_len +
			    (sizeof(struct spaceBitmapDesc) << 3),
			    sb->s_blocksize * 8);
}

static struct udf_bitmap *udf_sb_alloc_bitmap(struct super_block *sb, u32 index)
{
	struct udf_bitmap *bitmap;
	int nr_groups;
	int size;

	nr_groups = udf_compute_nr_groups(sb, index);
	size = sizeof(struct udf_bitmap) +
		(sizeof(struct buffer_head *) * nr_groups);

	if (size <= PAGE_SIZE)
		bitmap = kzalloc(size, GFP_KERNEL);
	else
		bitmap = vzalloc(size); /* TODO: get rid of vzalloc */

	if (bitmap == NULL) {
		udf_error(sb, __func__,
			  "Unable to allocate space for bitmap "
			  "and %d buffer_head pointers", nr_groups);
		return NULL;
	}

	bitmap->s_block_bitmap = (struct buffer_head **)(bitmap + 1);
	bitmap->s_nr_groups = nr_groups;
	return bitmap;
}

static int udf_fill_partdesc_info(struct super_block *sb,
		struct partitionDesc *p, int p_index)
{
	struct udf_part_map *map;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct partitionHeaderDesc *phd;

	map = &sbi->s_partmaps[p_index];

	map->s_partition_len = le32_to_cpu(p->partitionLength); /* blocks */
	map->s_partition_root = le32_to_cpu(p->partitionStartingLocation);

	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_READ_ONLY))
		map->s_partition_flags |= UDF_PART_FLAG_READ_ONLY;
	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_WRITE_ONCE))
		map->s_partition_flags |= UDF_PART_FLAG_WRITE_ONCE;
	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_REWRITABLE))
		map->s_partition_flags |= UDF_PART_FLAG_REWRITABLE;
	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_OVERWRITABLE))
		map->s_partition_flags |= UDF_PART_FLAG_OVERWRITABLE;

	udf_debug("Partition (%d type %x) starts at physical %d, "
		  "block length %d\n", p_index,
		  map->s_partition_type, map->s_partition_root,
		  map->s_partition_len);

	if (strcmp(p->partitionContents.ident, PD_PARTITION_CONTENTS_NSR02) &&
	    strcmp(p->partitionContents.ident, PD_PARTITION_CONTENTS_NSR03))
		return 0;

	phd = (struct partitionHeaderDesc *)p->partitionContentsUse;
	if (phd->unallocSpaceTable.extLength) {
		struct kernel_lb_addr loc = {
			.logicalBlockNum = le32_to_cpu(
				phd->unallocSpaceTable.extPosition),
			.partitionReferenceNum = p_index,
		};

		map->s_uspace.s_table = udf_iget(sb, &loc);
		if (!map->s_uspace.s_table) {
			udf_debug("cannot load unallocSpaceTable (part %d)\n",
					p_index);
			return 1;
		}
		map->s_partition_flags |= UDF_PART_FLAG_UNALLOC_TABLE;
		udf_debug("unallocSpaceTable (part %d) @ %ld\n",
				p_index, map->s_uspace.s_table->i_ino);
	}

	if (phd->unallocSpaceBitmap.extLength) {
		struct udf_bitmap *bitmap = udf_sb_alloc_bitmap(sb, p_index);
		if (!bitmap)
			return 1;
		map->s_uspace.s_bitmap = bitmap;
		bitmap->s_extLength = le32_to_cpu(
				phd->unallocSpaceBitmap.extLength);
		bitmap->s_extPosition = le32_to_cpu(
				phd->unallocSpaceBitmap.extPosition);
		map->s_partition_flags |= UDF_PART_FLAG_UNALLOC_BITMAP;
		udf_debug("unallocSpaceBitmap (part %d) @ %d\n", p_index,
						bitmap->s_extPosition);
	}

	if (phd->partitionIntegrityTable.extLength)
		udf_debug("partitionIntegrityTable (part %d)\n", p_index);

	if (phd->freedSpaceTable.extLength) {
		struct kernel_lb_addr loc = {
			.logicalBlockNum = le32_to_cpu(
				phd->freedSpaceTable.extPosition),
			.partitionReferenceNum = p_index,
		};

		map->s_fspace.s_table = udf_iget(sb, &loc);
		if (!map->s_fspace.s_table) {
			udf_debug("cannot load freedSpaceTable (part %d)\n",
				p_index);
			return 1;
		}

		map->s_partition_flags |= UDF_PART_FLAG_FREED_TABLE;
		udf_debug("freedSpaceTable (part %d) @ %ld\n",
				p_index, map->s_fspace.s_table->i_ino);
	}

	if (phd->freedSpaceBitmap.extLength) {
		struct udf_bitmap *bitmap = udf_sb_alloc_bitmap(sb, p_index);
		if (!bitmap)
			return 1;
		map->s_fspace.s_bitmap = bitmap;
		bitmap->s_extLength = le32_to_cpu(
				phd->freedSpaceBitmap.extLength);
		bitmap->s_extPosition = le32_to_cpu(
				phd->freedSpaceBitmap.extPosition);
		map->s_partition_flags |= UDF_PART_FLAG_FREED_BITMAP;
		udf_debug("freedSpaceBitmap (part %d) @ %d\n", p_index,
					bitmap->s_extPosition);
	}
	return 0;
}

static void udf_find_vat_block(struct super_block *sb, int p_index,
			       int type1_index, sector_t start_block)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct udf_part_map *map = &sbi->s_partmaps[p_index];
	sector_t vat_block;
	struct kernel_lb_addr ino;

	/*
	 * VAT file entry is in the last recorded block. Some broken disks have
	 * it a few blocks before so try a bit harder...
	 */
	ino.partitionReferenceNum = type1_index;
	for (vat_block = start_block;
	     vat_block >= map->s_partition_root &&
	     vat_block >= start_block - 3 &&
	     !sbi->s_vat_inode; vat_block--) {
		ino.logicalBlockNum = vat_block - map->s_partition_root;
		sbi->s_vat_inode = udf_iget(sb, &ino);
	}
}

static int udf_load_vat(struct super_block *sb, int p_index, int type1_index)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct udf_part_map *map = &sbi->s_partmaps[p_index];
	struct buffer_head *bh = NULL;
	struct udf_inode_info *vati;
	uint32_t pos;
	struct virtualAllocationTable20 *vat20;
	sector_t blocks = sb->s_bdev->bd_inode->i_size >> sb->s_blocksize_bits;

	udf_find_vat_block(sb, p_index, type1_index, sbi->s_last_block);
	if (!sbi->s_vat_inode &&
	    sbi->s_last_block != blocks - 1) {
		printk(KERN_NOTICE "UDF-fs: Failed to read VAT inode from the"
		       " last recorded block (%lu), retrying with the last "
		       "block of the device (%lu).\n",
		       (unsigned long)sbi->s_last_block,
		       (unsigned long)blocks - 1);
		udf_find_vat_block(sb, p_index, type1_index, blocks - 1);
	}
	if (!sbi->s_vat_inode)
		return 1;

	if (map->s_partition_type == UDF_VIRTUAL_MAP15) {
		map->s_type_specific.s_virtual.s_start_offset = 0;
		map->s_type_specific.s_virtual.s_num_entries =
			(sbi->s_vat_inode->i_size - 36) >> 2;
	} else if (map->s_partition_type == UDF_VIRTUAL_MAP20) {
		vati = UDF_I(sbi->s_vat_inode);
		if (vati->i_alloc_type != ICBTAG_FLAG_AD_IN_ICB) {
			pos = udf_block_map(sbi->s_vat_inode, 0);
			bh = sb_bread(sb, pos);
			if (!bh)
				return 1;
			vat20 = (struct virtualAllocationTable20 *)bh->b_data;
		} else {
			vat20 = (struct virtualAllocationTable20 *)
							vati->i_ext.i_data;
		}

		map->s_type_specific.s_virtual.s_start_offset =
			le16_to_cpu(vat20->lengthHeader);
		map->s_type_specific.s_virtual.s_num_entries =
			(sbi->s_vat_inode->i_size -
				map->s_type_specific.s_virtual.
					s_start_offset) >> 2;
		udf_release_data(bh);
	}
	return 0;
}

static int udf_load_partdesc(struct super_block *sb, sector_t block)
{
	struct buffer_head *bh;
	struct partitionDesc *p;
	struct udf_part_map *map;
	struct udf_sb_info *sbi = UDF_SB(sb);
	int i, type1_idx;
	uint16_t partitionNumber;
	uint16_t ident;
	int ret = 0;

	bh = udf_read_tagged(sb, block, block, &ident);
	if (!bh)
		return 1;
	if (ident != TAG_IDENT_PD)
		goto out_bh;

	p = (struct partitionDesc *)bh->b_data;
	partitionNumber = le16_to_cpu(p->partitionNumber);

	/* First scan for TYPE1, SPARABLE and METADATA partitions */
	for (i = 0; i < sbi->s_partitions; i++) {
		map = &sbi->s_partmaps[i];
		udf_debug("Searching map: (%d == %d)\n",
			  map->s_partition_num, partitionNumber);
		if (map->s_partition_num == partitionNumber &&
		    (map->s_partition_type == UDF_TYPE1_MAP15 ||
		     map->s_partition_type == UDF_SPARABLE_MAP15))
			break;
	}

	if (i >= sbi->s_partitions) {
		udf_debug("Partition (%d) not found in partition map\n",
			  partitionNumber);
		goto out_bh;
	}

	ret = udf_fill_partdesc_info(sb, p, i);

	/*
	 * Now rescan for VIRTUAL or METADATA partitions when SPARABLE and
	 * PHYSICAL partitions are already set up
	 */
	type1_idx = i;
	for (i = 0; i < sbi->s_partitions; i++) {
		map = &sbi->s_partmaps[i];

		if (map->s_partition_num == partitionNumber &&
		    (map->s_partition_type == UDF_VIRTUAL_MAP15 ||
		     map->s_partition_type == UDF_VIRTUAL_MAP20 ||
		     map->s_partition_type == UDF_METADATA_MAP25))
			break;
	}

	if (i >= sbi->s_partitions)
		goto out_bh;

	ret = udf_fill_partdesc_info(sb, p, i);
	if (ret)
		goto out_bh;

	if (map->s_partition_type == UDF_METADATA_MAP25) {
		ret = udf_load_metadata_files(sb, i);
		if (ret) {
			printk(KERN_ERR "UDF-fs: error loading MetaData "
			"partition map %d\n", i);
			goto out_bh;
		}
	} else {
		ret = udf_load_vat(sb, i, type1_idx);
		if (ret)
			goto out_bh;
		/*
		 * Mark filesystem read-only if we have a partition with
		 * virtual map since we don't handle writing to it (we
		 * overwrite blocks instead of relocating them).
		 */
		sb->s_flags |= MS_RDONLY;
		printk(KERN_NOTICE "UDF-fs: Filesystem marked read-only "
			"because writing to pseudooverwrite partition is "
			"not implemented.\n");
	}
out_bh:
	/* In case loading failed, we handle cleanup in udf_fill_super */
	udf_release_data(bh);
	return ret;
}

static int udf_load_logicalvol(struct super_block *sb, sector_t block,
			       struct kernel_lb_addr *fileset)
{
	struct logicalVolDesc *lvd;
	int i, j, offset;
	uint8_t type;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct genericPartitionMap *gpm;
	uint16_t ident;
	struct buffer_head *bh;
	int ret = 0;

	bh = udf_read_tagged(sb, block, block, &ident);
	if (!bh)
		return 1;
	BUG_ON(ident != TAG_IDENT_LVD);
	lvd = (struct logicalVolDesc *)bh->b_data;

	i = udf_sb_alloc_partition_maps(sb, le32_to_cpu(lvd->numPartitionMaps));
	if (i != 0) {
		ret = i;
		goto out_bh;
	}

	for (i = 0, offset = 0;
	     i < sbi->s_partitions && offset < le32_to_cpu(lvd->mapTableLength);
	     i++, offset += gpm->partitionMapLength) {
		struct udf_part_map *map = &sbi->s_partmaps[i];
		gpm = (struct genericPartitionMap *)
				&(lvd->partitionMaps[offset]);
		type = gpm->partitionMapType;
		if (type == 1) {
			struct genericPartitionMap1 *gpm1 =
				(struct genericPartitionMap1 *)gpm;
			map->s_partition_type = UDF_TYPE1_MAP15;
			map->s_volumeseqnum = le16_to_cpu(gpm1->volSeqNum);
			map->s_partition_num = le16_to_cpu(gpm1->partitionNum);
			map->s_partition_func = NULL;
		} else if (type == 2) {
			struct udfPartitionMap2 *upm2 =
						(struct udfPartitionMap2 *)gpm;
			if (!strncmp(upm2->partIdent.ident, UDF_ID_VIRTUAL,
						strlen(UDF_ID_VIRTUAL))) {
				u16 suf =
					le16_to_cpu(((__le16 *)upm2->partIdent.
							identSuffix)[0]);
				if (suf < 0x0200) {
					map->s_partition_type =
							UDF_VIRTUAL_MAP15;
					map->s_partition_func =
							udf_get_pblock_virt15;
				} else {
					map->s_partition_type =
							UDF_VIRTUAL_MAP20;
					map->s_partition_func =
							udf_get_pblock_virt20;
				}
			} else if (!strncmp(upm2->partIdent.ident,
						UDF_ID_SPARABLE,
						strlen(UDF_ID_SPARABLE))) {
				uint32_t loc;
				struct sparingTable *st;
				struct sparablePartitionMap *spm =
					(struct sparablePartitionMap *)gpm;

				map->s_partition_type = UDF_SPARABLE_MAP15;
				map->s_type_specific.s_sparing.s_packet_len =
						le16_to_cpu(spm->packetLength);
				for (j = 0; j < spm->numSparingTables; j++) {
					struct buffer_head *bh2;

					loc = le32_to_cpu(
						spm->locSparingTable[j]);
					bh2 = udf_read_tagged(sb, loc, loc,
							     &ident);
					map->s_type_specific.s_sparing.
							s_spar_map[j] = bh2;

					if (bh2 == NULL)
						continue;

					st = (struct sparingTable *)bh2->b_data;
					if (ident != 0 || strncmp(
						st->sparingIdent.ident,
						UDF_ID_SPARING,
						strlen(UDF_ID_SPARING))) {
						udf_release_data(bh2);
						map->s_type_specific.s_sparing.
							s_spar_map[j] = NULL;
					}
				}
				map->s_partition_func = udf_get_pblock_spar15;
			} else if (!strncmp(upm2->partIdent.ident,
						UDF_ID_METADATA,
						strlen(UDF_ID_METADATA))) {
				struct udf_meta_data *mdata =
					&map->s_type_specific.s_metadata;
				struct metadataPartitionMap *mdm =
						(struct metadataPartitionMap *)
						&(lvd->partitionMaps[offset]);
				udf_debug("Parsing Logical vol part %d "
					"type %d  id=%s\n", i, type,
					UDF_ID_METADATA);

				map->s_partition_type = UDF_METADATA_MAP25;
				map->s_partition_func = udf_get_pblock_meta25;

				mdata->s_meta_file_loc   =
					le32_to_cpu(mdm->metadataFileLoc);
				mdata->s_mirror_file_loc =
					le32_to_cpu(mdm->metadataMirrorFileLoc);
				mdata->s_bitmap_file_loc =
					le32_to_cpu(mdm->metadataBitmapFileLoc);
				mdata->s_alloc_unit_size =
					le32_to_cpu(mdm->allocUnitSize);
				mdata->s_align_unit_size =
					le16_to_cpu(mdm->alignUnitSize);
				mdata->s_dup_md_flag 	 =
					mdm->flags & 0x01;

				udf_debug("Metadata Ident suffix=0x%x\n",
					(le16_to_cpu(
					 ((__le16 *)
					      mdm->partIdent.identSuffix)[0])));
				udf_debug("Metadata part num=%d\n",
					le16_to_cpu(mdm->partitionNum));
				udf_debug("Metadata part alloc unit size=%d\n",
					le32_to_cpu(mdm->allocUnitSize));
				udf_debug("Metadata file loc=%d\n",
					le32_to_cpu(mdm->metadataFileLoc));
				udf_debug("Mirror file loc=%d\n",
				       le32_to_cpu(mdm->metadataMirrorFileLoc));
				udf_debug("Bitmap file loc=%d\n",
				       le32_to_cpu(mdm->metadataBitmapFileLoc));
				udf_debug("Duplicate Flag: %d %d\n",
					mdata->s_dup_md_flag, mdm->flags);
			} else {
				udf_debug("Unknown ident: %s\n",
					  upm2->partIdent.ident);
				continue;
			}
			map->s_volumeseqnum = le16_to_cpu(upm2->volSeqNum);
			map->s_partition_num = le16_to_cpu(upm2->partitionNum);
		}
		udf_debug("Partition (%d:%d) type %d on volume %d\n",
			  i, map->s_partition_num, type,
			  map->s_volumeseqnum);
	}

	if (fileset) {
		struct long_ad *la = (struct long_ad *)&(lvd->logicalVolContentsUse[0]);

		*fileset = lelb_to_cpu(la->extLocation);
		udf_debug("FileSet found in LogicalVolDesc at block=%d, "
			  "partition=%d\n", fileset->logicalBlockNum,
			  fileset->partitionReferenceNum);
	}
	if (lvd->integritySeqExt.extLength)
		udf_load_logicalvolint(sb, leea_to_cpu(lvd->integritySeqExt));

out_bh:
	udf_release_data(bh);
	return ret;
}

/*
 * udf_load_logicalvolint
 *
 */
static void udf_load_logicalvolint(struct super_block *sb, struct kernel_extent_ad loc)
{
	struct buffer_head *bh = NULL;
	uint16_t ident;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct logicalVolIntegrityDesc *lvid;

	while (loc.extLength > 0 &&
	       (bh = udf_read_tagged(sb, loc.extLocation,
				     loc.extLocation, &ident)) &&
	       ident == TAG_IDENT_LVID) {
		sbi->s_lvid_bh = bh;
		lvid = (struct logicalVolIntegrityDesc *)bh->b_data;

		if (lvid->nextIntegrityExt.extLength)
			udf_load_logicalvolint(sb,
				leea_to_cpu(lvid->nextIntegrityExt));

		if (sbi->s_lvid_bh != bh)
			udf_release_data(bh);
		loc.extLength -= sb->s_blocksize;
		loc.extLocation++;
	}
	if (sbi->s_lvid_bh != bh)
		udf_release_data(bh);
}

/*
 * udf_process_sequence
 *
 * PURPOSE
 *	Process a main/reserve volume descriptor sequence.
 *
 * PRE-CONDITIONS
 *	sb			Pointer to _locked_ superblock.
 *	block			First block of first extent of the sequence.
 *	lastblock		Lastblock of first extent of the sequence.
 *
 * HISTORY
 *	July 1, 1997 - Andrew E. Mileski
 *	Written, tested, and released.
 */
static noinline int udf_process_sequence(struct super_block *sb, long block,
				long lastblock, struct kernel_lb_addr *fileset)
{
	struct buffer_head *bh = NULL;
	struct udf_vds_record vds[VDS_POS_LENGTH];
	struct udf_vds_record *curr;
	struct generic_desc *gd;
	struct volDescPtr *vdp;
	int done = 0;
	uint32_t vdsn;
	uint16_t ident;
	long next_s = 0, next_e = 0;

	memset(vds, 0, sizeof(struct udf_vds_record) * VDS_POS_LENGTH);

	/*
	 * Read the main descriptor sequence and find which descriptors
	 * are in it.
	 */
	for (; (!done && block <= lastblock); block++) {

		bh = udf_read_tagged(sb, block, block, &ident);
		if (!bh) {
			printk(KERN_ERR "udf: Block %Lu of volume descriptor "
			       "sequence is corrupted or we could not read "
			       "it.\n", (unsigned long long)block);
			return 1;
		}

		/* Process each descriptor (ISO 13346 3/8.3-8.4) */
		gd = (struct generic_desc *)bh->b_data;
		vdsn = le32_to_cpu(gd->volDescSeqNum);
		switch (ident) {
		case TAG_IDENT_PVD: /* ISO 13346 3/10.1 */
			curr = &vds[VDS_POS_PRIMARY_VOL_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_VDP: /* ISO 13346 3/10.3 */
			curr = &vds[VDS_POS_VOL_DESC_PTR];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;

				vdp = (struct volDescPtr *)bh->b_data;
				next_s = le32_to_cpu(
					vdp->nextVolDescSeqExt.extLocation);
				next_e = le32_to_cpu(
					vdp->nextVolDescSeqExt.extLength);
				next_e = next_e >> sb->s_blocksize_bits;
				next_e += next_s;
			}
			break;
		case TAG_IDENT_IUVD: /* ISO 13346 3/10.4 */
			curr = &vds[VDS_POS_IMP_USE_VOL_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_PD: /* ISO 13346 3/10.5 */
			curr = &vds[VDS_POS_PARTITION_DESC];
			if (!curr->block)
				curr->block = block;
			break;
		case TAG_IDENT_LVD: /* ISO 13346 3/10.6 */
			curr = &vds[VDS_POS_LOGICAL_VOL_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_USD: /* ISO 13346 3/10.8 */
			curr = &vds[VDS_POS_UNALLOC_SPACE_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_TD: /* ISO 13346 3/10.9 */
			vds[VDS_POS_TERMINATING_DESC].block = block;
			if (next_e) {
				block = next_s;
				lastblock = next_e;
				next_s = next_e = 0;
			} else
				done = 1;
			break;
		}
		udf_release_data(bh);
	}
	/*
	 * Now read interesting descriptors again and process them
	 * in a suitable order
	 */
	if (!vds[VDS_POS_PRIMARY_VOL_DESC].block) {
		printk(KERN_ERR "udf: Primary Volume Descriptor not found!\n");
		return 1;
	}
	if (udf_load_pvoldesc(sb, vds[VDS_POS_PRIMARY_VOL_DESC].block))
		return 1;

	if (vds[VDS_POS_LOGICAL_VOL_DESC].block && udf_load_logicalvol(sb,
	    vds[VDS_POS_LOGICAL_VOL_DESC].block, fileset))
		return 1;

	if (vds[VDS_POS_PARTITION_DESC].block) {
		/*
		 * We rescan the whole descriptor sequence to find
		 * partition descriptor blocks and process them.
		 */
		for (block = vds[VDS_POS_PARTITION_DESC].block;
		     block < vds[VDS_POS_TERMINATING_DESC].block;
		     block++)
			if (udf_load_partdesc(sb, block))
				return 1;
	}

	return 0;
}

static int udf_load_sequence(struct super_block *sb, struct buffer_head *bh,
			     struct kernel_lb_addr *fileset)
{
	struct anchorVolDescPtr *anchor;
	long main_s, main_e, reserve_s, reserve_e;

	anchor = (struct anchorVolDescPtr *)bh->b_data;

	/* Locate the main sequence */
	main_s = le32_to_cpu(anchor->mainVolDescSeqExt.extLocation);
	main_e = le32_to_cpu(anchor->mainVolDescSeqExt.extLength);
	main_e = main_e >> sb->s_blocksize_bits;
	main_e += main_s;

	/* Locate the reserve sequence */
	reserve_s = le32_to_cpu(anchor->reserveVolDescSeqExt.extLocation);
	reserve_e = le32_to_cpu(anchor->reserveVolDescSeqExt.extLength);
	reserve_e = reserve_e >> sb->s_blocksize_bits;
	reserve_e += reserve_s;

	/* Process the main & reserve sequences */
	/* responsible for finding the PartitionDesc(s) */
	if (!udf_process_sequence(sb, main_s, main_e, fileset))
		return 1;
	return !udf_process_sequence(sb, reserve_s, reserve_e, fileset);
}

/*
 * Check whether there is an anchor block in the given block and
 * load Volume Descriptor Sequence if so.
 */
static int udf_check_anchor_block(struct super_block *sb, sector_t block,
				  struct kernel_lb_addr *fileset)
{
	struct buffer_head *bh;
	uint16_t ident;
	int ret;
	struct timeval t1, t2;
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_VARCONV) &&
	    udf_fixed_to_variable(block) >=
	    sb->s_bdev->bd_inode->i_size >> sb->s_blocksize_bits)
		return 0;
	jiffies_to_timeval(jiffies, &t1);
	bh = udf_read_tagged(sb, block, block, &ident);
	jiffies_to_timeval(jiffies, &t2);
	if (!bh && ((t2.tv_sec - t1.tv_sec) > 2)) {
        	timeouts++;
                udf_debug("%s: udf_read_tagged timeout block %d\n",
                          __FUNCTION__, (int)block);
        }
        else if (!bh) {
                read_errors++;
                udf_debug("%s: udf_read_tagged error block %d\n",
                          __FUNCTION__, (int)block);
        }
	if (!bh)
		return 0;
	if (ident != TAG_IDENT_AVDP) {
		udf_release_data(bh);
		return 0;
	}
	ret = udf_load_sequence(sb, bh, fileset);
	udf_release_data(bh);
	return ret;
}

/* Search for an anchor volume descriptor pointer */
static sector_t udf_scan_anchors(struct super_block *sb, sector_t lastblock,
				 struct kernel_lb_addr *fileset)
{
	sector_t last[6];
	int i;
	struct udf_sb_info *sbi = UDF_SB(sb);
	int last_count = 0;

	/* First try user provided anchor */
	if (sbi->s_anchor) {
		if (udf_check_anchor_block(sb, sbi->s_anchor, fileset))
			return lastblock;
	}
	/*
	 * according to spec, anchor is in either:
	 *     block 256
	 *     lastblock-256
	 *     lastblock
	 *  however, if the disc isn't closed, it could be 512.
	 */
	if (udf_check_anchor_block(sb, sbi->s_session + 256, fileset))
		return lastblock;
	/*
	 * The trouble is which block is the last one. Drives often misreport
	 * this so we try various possibilities.
	 */
	last[last_count++] = lastblock;
	if (lastblock >= 1)
		last[last_count++] = lastblock - 1;
	last[last_count++] = lastblock + 1;
	if (lastblock >= 2)
		last[last_count++] = lastblock - 2;
	if (lastblock >= 150)
		last[last_count++] = lastblock - 150;
	if (lastblock >= 152)
		last[last_count++] = lastblock - 152;

	for (i = 0; i < last_count; i++) {
		if (last[i] >= sb->s_bdev->bd_inode->i_size >>
				sb->s_blocksize_bits)
			continue;
		if (udf_check_anchor_block(sb, last[i], fileset))
			return last[i];
		if ((timeouts > 2) || (read_errors > 12)) {
                        udf_debug("%s: error limit1\n",
                                  __FUNCTION__);
                        timeouts = read_errors = 0;
                        break;
                }
		if (last[i] < 256)
			continue;
		if (udf_check_anchor_block(sb, last[i] - 256, fileset))
			return last[i];
		if ((timeouts > 2) || (read_errors > 12)) {
                        udf_debug("%s: error limit2\n",
                                  __FUNCTION__);
                        timeouts = read_errors = 0;
                        break;
                }
	}

	/* Finally try block 512 in case media is open */
	if (udf_check_anchor_block(sb, sbi->s_session + 512, fileset))
		return last[0];
	return 0;
}

/*
 * Find an anchor volume descriptor and load Volume Descriptor Sequence from
 * area specified by it. The function expects sbi->s_lastblock to be the last
 * block on the media.
 *
 * Return 1 if ok, 0 if not found.
 *
 */
static int udf_find_anchor(struct super_block *sb,
			   struct kernel_lb_addr *fileset)
{
	sector_t lastblock;
	struct udf_sb_info *sbi = UDF_SB(sb);

	timeouts = read_errors = 0;

	lastblock = udf_scan_anchors(sb, sbi->s_last_block, fileset);
	if (lastblock)
		goto out;

	/* No anchor found? Try VARCONV conversion of block numbers */
	UDF_SET_FLAG(sb, UDF_FLAG_VARCONV);
	/* Firstly, we try to not convert number of the last block */
	lastblock = udf_scan_anchors(sb,
				udf_variable_to_fixed(sbi->s_last_block),
				fileset);
	if (lastblock)
		goto out;

	/* Secondly, we try with converted number of the last block */
	lastblock = udf_scan_anchors(sb, sbi->s_last_block, fileset);
	if (!lastblock) {
		/* VARCONV didn't help. Clear it. */
		UDF_CLEAR_FLAG(sb, UDF_FLAG_VARCONV);
		return 0;
	}
out:
	sbi->s_last_block = lastblock;
	return 1;
}

/*
 * Check Volume Structure Descriptor, find Anchor block and load Volume
 * Descriptor Sequence
 */
static int udf_load_vrs(struct super_block *sb, struct udf_options *uopt,
			int silent, struct kernel_lb_addr *fileset)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	loff_t nsr_off;

	if (!sb_set_blocksize(sb, uopt->blocksize)) {
		if (!silent)
			printk(KERN_WARNING "UDF-fs: Bad block size\n");
		return 0;
	}
	sbi->s_last_block = uopt->lastblock;
	if (!uopt->novrs) {
		/* Check that it is NSR02 compliant */
		nsr_off = udf_check_vsd(sb);
		if (!nsr_off) {
			if (!silent)
				printk(KERN_WARNING "UDF-fs: No VRS found\n");
			return 0;
		}
		if (nsr_off == -1)
			udf_debug("Failed to read byte 32768. Assuming open "
				  "disc. Skipping validity check\n");
		if (!sbi->s_last_block)
			sbi->s_last_block = udf_get_last_block(sb);
	} else {
		udf_debug("Validity check skipped because of novrs option\n");
	}

	/* Look for anchor block and load Volume Descriptor Sequence */
	sbi->s_anchor = uopt->anchor;
	if (!udf_find_anchor(sb, fileset)) {
		if (!silent)
			printk(KERN_WARNING "UDF-fs: No anchor found\n");
		return 0;
	}
	return 1;
}

static void udf_open_lvid(struct super_block *sb)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct buffer_head *bh = sbi->s_lvid_bh;
	struct logicalVolIntegrityDesc *lvid;
	struct logicalVolIntegrityDescImpUse *lvidiu;

	if (!bh)
		return;

	mutex_lock(&sbi->s_alloc_mutex);
	lvid = (struct logicalVolIntegrityDesc *)bh->b_data;
	lvidiu = udf_sb_lvidiu(sbi);

	lvidiu->impIdent.identSuffix[0] = UDF_OS_CLASS_UNIX;
	lvidiu->impIdent.identSuffix[1] = UDF_OS_ID_LINUX;
	udf_time_to_disk_stamp(&lvid->recordingDateAndTime,
				CURRENT_TIME);
	lvid->integrityType = cpu_to_le32(LVID_INTEGRITY_TYPE_OPEN);

	lvid->descTag.descCRC = cpu_to_le16(
		crc_itu_t(0, (char *)lvid + sizeof(struct tag),
			le16_to_cpu(lvid->descTag.descCRCLength)));

	lvid->descTag.tagChecksum = udf_tag_checksum(&lvid->descTag);
	mark_buffer_dirty(bh);
	sbi->s_lvid_dirty = 0;
	mutex_unlock(&sbi->s_alloc_mutex);
}

static void udf_close_lvid(struct super_block *sb)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct buffer_head *bh = sbi->s_lvid_bh;
	struct logicalVolIntegrityDesc *lvid;
	struct logicalVolIntegrityDescImpUse *lvidiu;

	if (!bh)
		return;

	mutex_lock(&sbi->s_alloc_mutex);
	lvid = (struct logicalVolIntegrityDesc *)bh->b_data;
	lvidiu = udf_sb_lvidiu(sbi);
	lvidiu->impIdent.identSuffix[0] = UDF_OS_CLASS_UNIX;
	lvidiu->impIdent.identSuffix[1] = UDF_OS_ID_LINUX;
	udf_time_to_disk_stamp(&lvid->recordingDateAndTime, CURRENT_TIME);
	if (UDF_MAX_WRITE_VERSION > le16_to_cpu(lvidiu->maxUDFWriteRev))
		lvidiu->maxUDFWriteRev = cpu_to_le16(UDF_MAX_WRITE_VERSION);
	if (sbi->s_udfrev > le16_to_cpu(lvidiu->minUDFReadRev))
		lvidiu->minUDFReadRev = cpu_to_le16(sbi->s_udfrev);
	if (sbi->s_udfrev > le16_to_cpu(lvidiu->minUDFWriteRev))
		lvidiu->minUDFWriteRev = cpu_to_le16(sbi->s_udfrev);
	lvid->integrityType = cpu_to_le32(LVID_INTEGRITY_TYPE_CLOSE);

	lvid->descTag.descCRC = cpu_to_le16(
			crc_itu_t(0, (char *)lvid + sizeof(struct tag),
				le16_to_cpu(lvid->descTag.descCRCLength)));

	lvid->descTag.tagChecksum = udf_tag_checksum(&lvid->descTag);
	/*
	 * We set buffer uptodate unconditionally here to avoid spurious
	 * warnings from mark_buffer_dirty() when previous EIO has marked
	 * the buffer as !uptodate
	 */
	set_buffer_uptodate(bh);
	mark_buffer_dirty(bh);
	sbi->s_lvid_dirty = 0;
	mutex_unlock(&sbi->s_alloc_mutex);
}

u64 lvid_get_unique_id(struct super_block *sb)
{
	struct buffer_head *bh;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct logicalVolIntegrityDesc *lvid;
	struct logicalVolHeaderDesc *lvhd;
	u64 uniqueID;
	u64 ret;

	bh = sbi->s_lvid_bh;
	if (!bh)
		return 0;

	lvid = (struct logicalVolIntegrityDesc *)bh->b_data;
	lvhd = (struct logicalVolHeaderDesc *)lvid->logicalVolContentsUse;

	mutex_lock(&sbi->s_alloc_mutex);
	ret = uniqueID = le64_to_cpu(lvhd->uniqueID);
	if (!(++uniqueID & 0xFFFFFFFF))
		uniqueID += 16;
	lvhd->uniqueID = cpu_to_le64(uniqueID);
	mutex_unlock(&sbi->s_alloc_mutex);
	mark_buffer_dirty(bh);

	return ret;
}

static void udf_sb_free_bitmap(struct udf_bitmap *bitmap)
{
	int i;
	int nr_groups = bitmap->s_nr_groups;
	int size = sizeof(struct udf_bitmap) + (sizeof(struct buffer_head *) *
						nr_groups);

	for (i = 0; i < nr_groups; i++)
		if (bitmap->s_block_bitmap[i])
			udf_release_data(bitmap->s_block_bitmap[i]);

	if (size <= PAGE_SIZE)
		kfree(bitmap);
	else
		vfree(bitmap);
}

static void udf_free_partition(struct udf_part_map *map)
{
	int i;
	struct udf_meta_data *mdata;

	if (map->s_partition_flags & UDF_PART_FLAG_UNALLOC_TABLE)
		iput(map->s_uspace.s_table);
	if (map->s_partition_flags & UDF_PART_FLAG_FREED_TABLE)
		iput(map->s_fspace.s_table);
	if (map->s_partition_flags & UDF_PART_FLAG_UNALLOC_BITMAP)
		udf_sb_free_bitmap(map->s_uspace.s_bitmap);
	if (map->s_partition_flags & UDF_PART_FLAG_FREED_BITMAP)
		udf_sb_free_bitmap(map->s_fspace.s_bitmap);
	if (map->s_partition_type == UDF_SPARABLE_MAP15)
		for (i = 0; i < 4; i++)
			udf_release_data(map->s_type_specific.s_sparing.s_spar_map[i]);
	else if (map->s_partition_type == UDF_METADATA_MAP25) {
		mdata = &map->s_type_specific.s_metadata;
		iput(mdata->s_metadata_fe);
		mdata->s_metadata_fe = NULL;

		iput(mdata->s_mirror_fe);
		mdata->s_mirror_fe = NULL;

		iput(mdata->s_bitmap_fe);
		mdata->s_bitmap_fe = NULL;
	}
}

static int udf_fill_super(struct super_block *sb, void *options, int silent)
{
	int i;
	int ret;
	struct inode *inode = NULL;
	struct udf_options uopt;
	struct kernel_lb_addr rootdir, fileset;
	struct udf_sb_info *sbi;

	uopt.flags = (1 << UDF_FLAG_USE_AD_IN_ICB) | (1 << UDF_FLAG_STRICT);
	uopt.uid = -1;
	uopt.gid = -1;
	uopt.umask = 0;
	uopt.fmode = UDF_INVALID_MODE;
	uopt.dmode = UDF_INVALID_MODE;

	sbi = kzalloc(sizeof(struct udf_sb_info), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;

	sb->s_fs_info = sbi;

	mutex_init(&sbi->s_alloc_mutex);

	if (!udf_parse_options((char *)options, &uopt, false))
		goto error_out;
	bdrom_metadata_cache_on_init_udf(&g_sb_read_cache, uopt.bdrom); //by gyu
	if (uopt.flags & (1 << UDF_FLAG_UTF8) &&
	    uopt.flags & (1 << UDF_FLAG_NLS_MAP)) {
		udf_error(sb, "udf_read_super",
			  "utf8 cannot be combined with iocharset\n");
		goto error_out;
	}
#ifdef CONFIG_UDF_NLS
	if ((uopt.flags & (1 << UDF_FLAG_NLS_MAP)) && !uopt.nls_map) {
		uopt.nls_map = load_nls_default();
		if (!uopt.nls_map)
			uopt.flags &= ~(1 << UDF_FLAG_NLS_MAP);
		else
			udf_debug("Using default NLS map\n");
	}
#endif
	if (!(uopt.flags & (1 << UDF_FLAG_NLS_MAP)))
		uopt.flags |= (1 << UDF_FLAG_UTF8);

	fileset.logicalBlockNum = 0xFFFFFFFF;
	fileset.partitionReferenceNum = 0xFFFF;

	sbi->s_flags = uopt.flags;
	sbi->s_uid = uopt.uid;
	sbi->s_gid = uopt.gid;
	sbi->s_umask = uopt.umask;
	sbi->s_fmode = uopt.fmode;
	sbi->s_dmode = uopt.dmode;
	sbi->s_nls_map = uopt.nls_map;
	rwlock_init(&sbi->s_cred_lock);

	if (uopt.session == 0xFFFFFFFF)
		sbi->s_session = udf_get_last_session(sb);
	else
		sbi->s_session = uopt.session;

	udf_debug("Multi-session=%d\n", sbi->s_session);

	/* Fill in the rest of the superblock */
	sb->s_op = &udf_sb_ops;
	sb->s_export_op = &udf_export_ops;

	sb->s_dirt = 0;
	sb->s_magic = UDF_SUPER_MAGIC;
	sb->s_time_gran = 1000;

	if (uopt.flags & (1 << UDF_FLAG_BLOCKSIZE_SET)) {
		ret = udf_load_vrs(sb, &uopt, silent, &fileset);
	} else {
		uopt.blocksize = bdev_logical_block_size(sb->s_bdev);
		ret = udf_load_vrs(sb, &uopt, silent, &fileset);
		if (!ret && uopt.blocksize != UDF_DEFAULT_BLOCKSIZE) {
			if (!silent)
				printk(KERN_NOTICE
				       "UDF-fs: Rescanning with blocksize "
				       "%d\n", UDF_DEFAULT_BLOCKSIZE);
			uopt.blocksize = UDF_DEFAULT_BLOCKSIZE;
			ret = udf_load_vrs(sb, &uopt, silent, &fileset);
		}
	}
	if (!ret) {
		printk(KERN_WARNING "UDF-fs: No partition found (1)\n");
		goto error_out;
	}

	udf_debug("Lastblock=%d\n", sbi->s_last_block);

	if (sbi->s_lvid_bh) {
		struct logicalVolIntegrityDescImpUse *lvidiu =
							udf_sb_lvidiu(sbi);
		uint16_t minUDFReadRev = le16_to_cpu(lvidiu->minUDFReadRev);
		uint16_t minUDFWriteRev = le16_to_cpu(lvidiu->minUDFWriteRev);
		/* uint16_t maxUDFWriteRev =
				le16_to_cpu(lvidiu->maxUDFWriteRev); */

		if (minUDFReadRev > UDF_MAX_READ_VERSION) {
			printk(KERN_ERR "UDF-fs: minUDFReadRev=%x "
					"(max is %x)\n",
			       le16_to_cpu(lvidiu->minUDFReadRev),
			       UDF_MAX_READ_VERSION);
			goto error_out;
		} else if (minUDFWriteRev > UDF_MAX_WRITE_VERSION)
			sb->s_flags |= MS_RDONLY;

		sbi->s_udfrev = minUDFWriteRev;

		if (minUDFReadRev >= UDF_VERS_USE_EXTENDED_FE)
			UDF_SET_FLAG(sb, UDF_FLAG_USE_EXTENDED_FE);
		if (minUDFReadRev >= UDF_VERS_USE_STREAMS)
			UDF_SET_FLAG(sb, UDF_FLAG_USE_STREAMS);
	}

	if (!sbi->s_partitions) {
		printk(KERN_WARNING "UDF-fs: No partition found (2)\n");
		goto error_out;
	}

	if (sbi->s_partmaps[sbi->s_partition].s_partition_flags &
			UDF_PART_FLAG_READ_ONLY) {
		printk(KERN_NOTICE "UDF-fs: Partition marked readonly; "
				   "forcing readonly mount\n");
		sb->s_flags |= MS_RDONLY;
	}

	if (udf_find_fileset(sb, &fileset, &rootdir)) {
		printk(KERN_WARNING "UDF-fs: No fileset found\n");
		goto error_out;
	}

	if (!silent) {
		struct timestamp ts;
		udf_time_to_disk_stamp(&ts, sbi->s_record_time);
		udf_info("UDF: Mounting volume '%s', "
			 "timestamp %04u/%02u/%02u %02u:%02u (%x)\n",
			 sbi->s_volume_ident, le16_to_cpu(ts.year), ts.month, ts.day,
			 ts.hour, ts.minute, le16_to_cpu(ts.typeAndTimezone));
	}
	if (!(sb->s_flags & MS_RDONLY))
		udf_open_lvid(sb);

	/* Assign the root inode */
	/* assign inodes by physical block number */
	/* perhaps it's not extensible enough, but for now ... */
	inode = udf_iget(sb, &rootdir);
	if (!inode) {
		printk(KERN_ERR "UDF-fs: Error in udf_iget, block=%d, "
				"partition=%d\n",
		       rootdir.logicalBlockNum, rootdir.partitionReferenceNum);
		goto error_out;
	}

	/* Allocate a dentry for the root inode */
	sb->s_root = d_alloc_root(inode);
	if (!sb->s_root) {
		printk(KERN_ERR "UDF-fs: Couldn't allocate root dentry\n");
		iput(inode);
		goto error_out;
	}
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	return 0;

error_out:
	if (sbi->s_vat_inode)
		iput(sbi->s_vat_inode);
	if (sbi->s_partitions)
		for (i = 0; i < sbi->s_partitions; i++)
			udf_free_partition(&sbi->s_partmaps[i]);
#ifdef CONFIG_UDF_NLS
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP))
		unload_nls(sbi->s_nls_map);
#endif
	if (!(sb->s_flags & MS_RDONLY))
		udf_close_lvid(sb);
	udf_release_data(sbi->s_lvid_bh);

	bdrom_metadata_cache_cleanup(&g_sb_read_cache);
	kfree(sbi->s_partmaps);
	kfree(sbi);
	sb->s_fs_info = NULL;

	return -EINVAL;
}

static void udf_error(struct super_block *sb, const char *function,
		      const char *fmt, ...)
{
	va_list args;

	if (!(sb->s_flags & MS_RDONLY)) {
		/* mark sb error */
		sb->s_dirt = 1;
	}
	va_start(args, fmt);
	vsnprintf(error_buf, sizeof(error_buf), fmt, args);
	va_end(args);
	printk(KERN_CRIT "UDF-fs error (device %s): %s: %s\n",
		sb->s_id, function, error_buf);
}

void udf_warning(struct super_block *sb, const char *function,
		 const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(error_buf, sizeof(error_buf), fmt, args);
	va_end(args);
	printk(KERN_WARNING "UDF-fs warning (device %s): %s: %s\n",
	       sb->s_id, function, error_buf);
}

static void udf_put_super(struct super_block *sb)
{
	int i;
	struct udf_sb_info *sbi;

	sbi = UDF_SB(sb);

	if (sbi->s_vat_inode)
		iput(sbi->s_vat_inode);
	if (sbi->s_partitions)
		for (i = 0; i < sbi->s_partitions; i++)
			udf_free_partition(&sbi->s_partmaps[i]);
#ifdef CONFIG_UDF_NLS
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP))
		unload_nls(sbi->s_nls_map);
#endif
	if (!(sb->s_flags & MS_RDONLY))
		udf_close_lvid(sb);
	udf_release_data(sbi->s_lvid_bh);
	bdrom_metadata_cache_cleanup(&g_sb_read_cache); //by gyu
	kfree(sbi->s_partmaps);
	kfree(sb->s_fs_info);
	sb->s_fs_info = NULL;
}

static int udf_sync_fs(struct super_block *sb, int wait)
{
	struct udf_sb_info *sbi = UDF_SB(sb);

	mutex_lock(&sbi->s_alloc_mutex);
	if (sbi->s_lvid_dirty) {
		/*
		 * Blockdevice will be synced later so we don't have to submit
		 * the buffer for IO
		 */
		mark_buffer_dirty(sbi->s_lvid_bh);
		sb->s_dirt = 0;
		sbi->s_lvid_dirty = 0;
	}
	mutex_unlock(&sbi->s_alloc_mutex);

	return 0;
}

static int udf_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct logicalVolIntegrityDescImpUse *lvidiu;
	u64 id = huge_encode_dev(sb->s_bdev->bd_dev);

	if (sbi->s_lvid_bh != NULL)
		lvidiu = udf_sb_lvidiu(sbi);
	else
		lvidiu = NULL;

	buf->f_type = UDF_SUPER_MAGIC;
	buf->f_bsize = sb->s_blocksize;
	buf->f_blocks = sbi->s_partmaps[sbi->s_partition].s_partition_len;
	buf->f_bfree = udf_count_free(sb);
	buf->f_bavail = buf->f_bfree;
	buf->f_files = (lvidiu != NULL ? (le32_to_cpu(lvidiu->numFiles) +
					  le32_to_cpu(lvidiu->numDirs)) : 0)
			+ buf->f_bfree;
	buf->f_ffree = buf->f_bfree;
	buf->f_namelen = UDF_NAME_LEN - 2;
	buf->f_fsid.val[0] = (u32)id;
	buf->f_fsid.val[1] = (u32)(id >> 32);

	return 0;
}

static unsigned int udf_count_free_bitmap(struct super_block *sb,
					  struct udf_bitmap *bitmap)
{
	struct buffer_head *bh = NULL;
	unsigned int accum = 0;
	int index;
	int block = 0, newblock;
	struct kernel_lb_addr loc;
	uint32_t bytes;
	uint8_t *ptr;
	uint16_t ident;
	struct spaceBitmapDesc *bm;

	loc.logicalBlockNum = bitmap->s_extPosition;
	loc.partitionReferenceNum = UDF_SB(sb)->s_partition;
	bh = udf_read_ptagged(sb, &loc, 0, &ident);

	if (!bh) {
		printk(KERN_ERR "udf: udf_count_free failed\n");
		goto out;
	} else if (ident != TAG_IDENT_SBD) {
		udf_release_data(bh);
		printk(KERN_ERR "udf: udf_count_free failed\n");
		goto out;
	}

	bm = (struct spaceBitmapDesc *)bh->b_data;
	bytes = le32_to_cpu(bm->numOfBytes);
	index = sizeof(struct spaceBitmapDesc); /* offset in first block only */
	ptr = (uint8_t *)bh->b_data;

	while (bytes > 0) {
		u32 cur_bytes = min_t(u32, bytes, sb->s_blocksize - index);
		accum += bitmap_weight((const unsigned long *)(ptr + index),
					cur_bytes * 8);
		bytes -= cur_bytes;
		if (bytes) {
			udf_release_data(bh);
			newblock = udf_get_lb_pblock(sb, &loc, ++block);
			bh = udf_tread(sb, newblock);
			if (!bh) {
				udf_debug("read failed\n");
				goto out;
			}
			index = 0;
			ptr = (uint8_t *)bh->b_data;
		}
	}
	udf_release_data(bh);
out:
	return accum;
}

static unsigned int udf_count_free_table(struct super_block *sb,
					 struct inode *table)
{
	unsigned int accum = 0;
	uint32_t elen;
	struct kernel_lb_addr eloc;
	int8_t etype;
	struct extent_position epos;

	mutex_lock(&UDF_SB(sb)->s_alloc_mutex);
	epos.block = UDF_I(table)->i_location;
	epos.offset = sizeof(struct unallocSpaceEntry);
	epos.bh = NULL;

	while ((etype = udf_next_aext(table, &epos, &eloc, &elen, 1)) != -1)
		accum += (elen >> table->i_sb->s_blocksize_bits);

	udf_release_data(epos.bh);
	mutex_unlock(&UDF_SB(sb)->s_alloc_mutex);

	return accum;
}

static unsigned int udf_count_free(struct super_block *sb)
{
	unsigned int accum = 0;
	struct udf_sb_info *sbi;
	struct udf_part_map *map;

	sbi = UDF_SB(sb);
	if (sbi->s_lvid_bh) {
		struct logicalVolIntegrityDesc *lvid =
			(struct logicalVolIntegrityDesc *)
			sbi->s_lvid_bh->b_data;
		if (le32_to_cpu(lvid->numOfPartitions) > sbi->s_partition) {
			accum = le32_to_cpu(
					lvid->freeSpaceTable[sbi->s_partition]);
			if (accum == 0xFFFFFFFF)
				accum = 0;
		}
	}

	if (accum)
		return accum;

	map = &sbi->s_partmaps[sbi->s_partition];
	if (map->s_partition_flags & UDF_PART_FLAG_UNALLOC_BITMAP) {
		accum += udf_count_free_bitmap(sb,
					       map->s_uspace.s_bitmap);
	}
	if (map->s_partition_flags & UDF_PART_FLAG_FREED_BITMAP) {
		accum += udf_count_free_bitmap(sb,
					       map->s_fspace.s_bitmap);
	}
	if (accum)
		return accum;

	if (map->s_partition_flags & UDF_PART_FLAG_UNALLOC_TABLE) {
		accum += udf_count_free_table(sb,
					      map->s_uspace.s_table);
	}
	if (map->s_partition_flags & UDF_PART_FLAG_FREED_TABLE) {
		accum += udf_count_free_table(sb,
					      map->s_fspace.s_table);
	}

	return accum;
}
