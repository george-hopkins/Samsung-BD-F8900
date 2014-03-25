/* fs/fat/nfs.c
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/exportfs.h>
#include "fat.h"

/**
 * Look up a directory inode given its starting cluster.
 */
static struct inode *fat_dget(struct super_block *sb, int i_logstart)
{
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	struct hlist_head *head;
	struct hlist_node *_p;
	struct msdos_inode_info *i;
	struct inode *inode = NULL;

	head = sbi->dir_hashtable + fat_dir_hash(i_logstart);
	spin_lock(&sbi->dir_hash_lock);
	hlist_for_each_entry(i, _p, head, i_dir_hash) {
		BUG_ON(i->vfs_inode.i_sb != sb);
		if (i->i_logstart != i_logstart)
			continue;
		inode = igrab(&i->vfs_inode);
		if (inode)
			break;
	}
	spin_unlock(&sbi->dir_hash_lock);
	return inode;
}

static struct inode *fat_nfs_get_inode(struct super_block *sb,
				       u64 ino, u32 generation)
{
	struct inode *inode;

	if ((ino < MSDOS_ROOT_INO) || (ino == MSDOS_FSINFO_INO))
		return NULL;

	inode = ilookup(sb, ino);
	if (inode && generation && (inode->i_generation != generation)) {
		iput(inode);
		inode = NULL;
	}
	if (inode == NULL && MSDOS_SB(sb)->options.nfs) {
		struct buffer_head *bh = NULL;
		struct msdos_dir_entry *de ;
		loff_t i_pos = (loff_t)ino;
		int bits = MSDOS_SB(sb)->dir_per_block_bits;
		loff_t blocknr = i_pos >> bits;
		bh = sb_bread(sb, blocknr);
		if (!bh) {
			fat_msg(sb, KERN_ERR,
				"unable to read block(%llu) for building NFS inode",
				(llu)blocknr);
			return inode;
		}
		de = (struct msdos_dir_entry *)bh->b_data;
		/* If a file is deleted on server and client is not updated
		 * yet, we must not build the inode upon a lookup call.
		 */
		if (de[i_pos&((1<<bits)-1)].name[0] == DELETED_FLAG)

			inode = NULL;
		else
			inode = fat_build_inode(sb, &de[i_pos&((1<<bits)-1)],
					i_pos);
		brelse(bh);
	}

	return inode;
}

/**
 * Map a NFS file handle to a corresponding dentry.
 * The dentry may or may not be connected to the filesystem root.
 */
struct dentry *fat_fh_to_dentry(struct super_block *sb, struct fid *fid,
				int fh_len, int fh_type)
{
	return generic_fh_to_dentry(sb, fid, fh_len, fh_type,
				    fat_nfs_get_inode);
}

/*
 * Find the parent for a file specified by NFS handle.
 * This requires that the handle contain the i_ino of the parent.
 */
struct dentry *fat_fh_to_parent(struct super_block *sb, struct fid *fid,
				int fh_len, int fh_type)
{
	return generic_fh_to_parent(sb, fid, fh_len, fh_type,
				    fat_nfs_get_inode);
}

/*
 * Read the directory entries of 'search_clus' and find the entry
 * which contains 'match_ipos' for the starting cluster.If the entry
 * is found, rebuild its inode.
 */
static struct inode *fat_traverse_cluster(struct super_block *sb,
				int search_clus, int match_ipos)
{
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	struct buffer_head *bh;
	sector_t blknr;
	int parent_ipos, search_ipos;
	int i;
	struct msdos_dir_entry *de;
	struct inode *inode = NULL;
	int iterations = sbi->cluster_size >> sb->s_blocksize_bits;
	blknr = fat_clus_to_blknr(sbi, search_clus);

	do {
		bh = sb_bread(sb, blknr);
		if (!bh) {
			fat_msg(sb, KERN_ERR,
				"NFS:unable to read block(%llu) while traversing cluster(%d)",
				(llu)blknr, search_clus);
			inode = ERR_PTR(-EIO);
			goto out;
		}
		de = (struct msdos_dir_entry *)bh->b_data;
		for (i = 0; i < sbi->dir_per_block; i++) {
			if (de[i].name[0] == FAT_ENT_FREE) {
				/*Reached end of directory*/
				brelse(bh);
				inode = ERR_PTR(-ENODATA);
				goto out;
			}
			if (de[i].name[0] == DELETED_FLAG)
				continue;
			if (de[i].attr == ATTR_EXT)
				continue;
			if (!(de[i].attr & ATTR_DIR))
				continue;
			else {
				search_ipos = fat_get_start(sbi, &de[i]);
				if (search_ipos == match_ipos) {
					/*Success.Now build the inode*/
					parent_ipos = (loff_t)i +
					 (blknr << sbi->dir_per_block_bits);
					inode = fat_build_inode(sb, &de[i],
								parent_ipos);
					brelse(bh);
					goto out;
				}
			}
		}
		brelse(bh);
		blknr += 1;
	} while (--iterations > 0);
out:
	return inode;
}

/*
 * Find the parent for a directory that is not currently connected to
 * the filesystem root.
 *
 * On entry, the caller holds child_dir->d_inode->i_mutex.
 */
struct dentry *fat_get_parent(struct dentry *child_dir)
{
	struct super_block *sb = child_dir->d_sb;
	struct buffer_head *dotdot_bh = NULL, *parent_bh = NULL;
	struct msdos_dir_entry *de;
	struct fat_entry fatent;
	struct inode *parent_inode = NULL;
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	int parent_logstart;
	int search_clus, clus_to_match, clus_count = 0;
	sector_t blknr;
	const int limit = sb->s_maxbytes >> MSDOS_SB(sb)->cluster_bits;

	if (!fat_get_dotdot_entry(child_dir->d_inode, &dotdot_bh, &de)) {
		parent_logstart = fat_get_start(sbi, de);
		parent_inode = fat_dget(sb, parent_logstart);
		if (parent_inode || !sbi->options.nfs)
			goto out;
		if (!parent_logstart)
			/*logstart of dotdot entry is zero if
			* if the directory's parent is root
			*/
			parent_inode = sb->s_root->d_inode;
		else {
			blknr = fat_clus_to_blknr(sbi, parent_logstart);
			parent_bh = sb_bread(sb, blknr);
			if (!parent_bh) {
				fat_msg(sb, KERN_ERR,
					"NFS:unable to read cluster of parent directory");
				goto out;
			}
			de = (struct msdos_dir_entry *) parent_bh->b_data;
			clus_to_match = fat_get_start(sbi, &de[0]);
			search_clus = fat_get_start(sbi, &de[1]);
			if (!search_clus) {
				if (sbi->fat_bits == 32)
					search_clus = sbi->root_cluster;
				else
				/* sbi->root_cluster is valid only for FAT32.
				 * Compute search_cluster such that when
				 * fat_traverse_cluster() is called for
				 * FAT 12/16, fat_clus_to_blknr() in it gives
				 * us sbi->dir_start
				 */
				search_clus = ((int)(sbi->dir_start - sbi->data_start)
						     >> ilog2(sbi->sec_per_clus))
						     + FAT_START_ENT;
			}
			brelse(parent_bh);
			do {
				parent_inode =  fat_traverse_cluster(sb,
						search_clus, clus_to_match);
				if (IS_ERR(parent_inode) || parent_inode)
					break;
				if (++clus_count > limit) {
					fat_fs_error_ratelimit(sb,
					"%s: detected the cluster chain loop"
					" while reading directory entries from"
					" cluster %d", __func__, search_clus);
					parent_inode = ERR_PTR(-EIO);
					break;
				}
				fatent_init(&fatent);
				search_clus = fat_ent_read(sb, &fatent,
							   search_clus);
				fatent_brelse(&fatent);
				if (search_clus < 0 ||
				    search_clus == FAT_ENT_FREE)
					break;
			} while (search_clus != FAT_ENT_EOF);
		}
	}
out:
	brelse(dotdot_bh);

	return d_obtain_alias(parent_inode);
}
