/**
 * @file	fs/emmcfs/procfs.c
 * @brief	Proc info support for eMMCFS.
 * @author	TODO
 * @date	TODO
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 * TODO: Detailed description
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

#include "emmcfs.h"

#ifdef CONFIG_EMMCFS_PROC_INFO
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/genhd.h>
#include <linux/vmalloc.h>

/**
 * @brief 	Global list for eMMCFS superblocks used by emmcfs_getbacktrace.
 */
struct sb_list {
	struct emmcfs_sb_info *element;
	struct sb_list *next;
} emmcfs_sb_list;

/**
 * @brief 	Proc directory entry for eMMCFS.
 */

struct proc_dir_entry *emmcfs_procdir_entry;

/**
 * @brief 	Initializes eMMCFS superblocks list.
 * @return	Returns 0 on success, errno on failure.
 */

int emmcfs_init_sb_list(void)
{
	emmcfs_sb_list.element = NULL;
	emmcfs_sb_list.next = NULL;
	return 0;
}

/**
 * @brief 		Adds eMMCFS superblock to global superblock list.
 * @param [in] 	sbi	Superblock that will be placed to list.
 * @return		Returns 0 on success, errno on failure.
 */
int emmcfs_add_sb_to_list(struct emmcfs_sb_info *sbi)
{
	struct sb_list *curr = &emmcfs_sb_list;
	while (curr->next)
		curr = curr->next;

	if (curr->element) {
		curr->next = kzalloc(sizeof(struct sb_list), GFP_KERNEL);
		if (!curr->next)
			return -ENOMEM;
		curr = curr->next;
	}
	curr->element = sbi;
	curr->next = NULL;
	return 0;
}

/**
 * @brief 		Removes eMMCFS superblock from global superblock list.
 * @param [in] 	sbi	Superblock that will be removed.
 * @return		Returns 0 on success, errno on failure.
 */
int emmcfs_remove_from_list(struct emmcfs_sb_info *sbi)
{
	struct sb_list *curr = &emmcfs_sb_list;
	struct sb_list *prev = NULL;

	while (curr->element != sbi) {
		if(!curr->next)
			return -ENOENT;
		prev = curr;
		curr = curr->next;
	}

	if (prev)
		prev->next = curr->next;
	else
		curr->element = NULL;
	return 0;
}

/**
 * @brief 		Checks log buffer for emptiness.
 * @param [in] 	sbi	Superblock that will be placed to list.
 * @return		Returns 0 if buffer is empty, 1 otherwise.
 */
static int emmcfs_check_buffer_empty(struct emmcfs_log_buffer *buffer)
{
	return (buffer->end == ((buffer->start + 1) % BUFFER_LENGTH));
}

/**
 * @brief			Adds function call/exit to the logging buffer.
 * @param [in,out] 	buffer	Buffer where function call/exit will be placed.
 * @param [in] 		name	Function that's logged.
 * @param [in] 		flag	Mark function call or exit.
 * @return			Returns 0 on success, errno on failure.
 */
int emmcfs_function_add(struct emmcfs_log_buffer *buffer,
		const char *name, int8_t flag, int ret)
{
	int len;
	struct timeval time;
	int pid;

	do_gettimeofday(&time);
	pid = flag * current->pid;
	len = strlen(name);
	if (len >= MAX_FUNCTION_LENGTH)
		return -ENAMETOOLONG;

	spin_lock(&buffer->sbi->nk->nest_log_lock);
	if (flag > 0)
		emmcfs_add_or_inc_data(buffer->sbi, EMMCFS_NEST);
	spin_unlock(&buffer->sbi->nk->nest_log_lock);
	buffer->elements[buffer->end].nest =
		*emmcfs_get_data(buffer->sbi->nk, EMMCFS_NEST);

	buffer->elements[buffer->end].time = time;
	buffer->elements[buffer->end].pid = pid;
	buffer->elements[buffer->end].ret_code = ret;
	memcpy(buffer->elements[buffer->end].name, name, len + 1);

	buffer->end = (buffer->end + 1) % BUFFER_LENGTH;

	if (buffer->end == buffer->start)
		buffer->start = (buffer->start + 1) % BUFFER_LENGTH;
	spin_lock(&buffer->sbi->nk->nest_log_lock);
	if (flag < 0)
		emmcfs_dec_data(buffer->sbi, EMMCFS_NEST);
	spin_unlock(&buffer->sbi->nk->nest_log_lock);

	return 0;
}

/**
 * @brief 		Gets first element of buffer.
 * @param [in] 	buf	Buffer which first element will be returned.
 * @return		Returns first element of the buffer.
 */
static inline struct emmcfs_log_buffer_data *get_head(
		struct emmcfs_log_buffer *buf)
{
	return &buf->elements[buf->start];
}

/**
 * @brief 		Returns table title for backtrace.
 * @param [in] 	none
 * @return		Returns string with table title.
 */
static inline char *get_table_title(void)
{
	return "PID\tSec\tmkSec\tName";
}

/**
 * @brief 		Return mutexes usage statistic.
 * @param [in] 	nk	Nesting keeper from mutexes usage statistic that
 *			will be got.
 * @return		Returns string with table title.
 */
static char *get_mutexes(struct emmcfs_nesting_keeper *nk)
{
	char *res;
	int i;
	const char *string_mutexes[] = {"ext tree r", "cat tree r",
			"transaction r", "task log r", "fs r", "fsm r",
			"ext tree w", "cat tree w", "transaction w",
			"task log w", "fs w", "fsm w"};

	res = kzalloc(sizeof(char)*1000, GFP_KERNEL);
	if (!res)
		return ERR_PTR(-ENOMEM);
	for (i = 0; i < EMMCFS_MUTEX_COUNT; i++) {
		if(nk->size[i]) {
			int *pids = emmcfs_get_data(nk, i + 1);
			int j;

			if (IS_ERR(pids)) {
				kfree(res);
				return ((char*)pids);
			}

			for (j = 0; j < nk->size[i]; ++j) {
				sprintf(res+strlen(res),
					"%s lock used by %d\n",
					string_mutexes[i], pids[j]);
			}
			kfree(pids);
		}
	}
	return res;
}

/**
 * @brief 		Converts log buffer head element to string.
 * @param [in] 	buffer	Log buffer which head element will be converted.
 * @return	void
 */
static void emmcfs_element_to_string(struct emmcfs_log_buffer *buffer)
{
	int i;
	sprintf(buffer->element_to_string, "%ld\t", abs(get_head(buffer)->pid));

	snprintf(buffer->element_to_string + strlen(buffer->element_to_string),
			EMMCFS_MAX_STAT_LENGTH, "%ld\t%ld\t",
			(long)get_head(buffer)->time.tv_sec % 1000,
			(long)get_head(buffer)->time.tv_usec / 1000);

	sprintf(buffer->element_to_string +
		strlen(buffer->element_to_string), "\t");

	for (i = 0; i < get_head(buffer)->nest; i++) {
		sprintf(buffer->element_to_string +
			strlen(buffer->element_to_string), "|");
	}

	sprintf(buffer->element_to_string + strlen(buffer->element_to_string),
			"%s%s", get_head(buffer)->pid > 0? "->" : "<-",
					get_head(buffer)->name);

	if(get_head(buffer)->pid < 0)
		sprintf(buffer->element_to_string +
				strlen(buffer->element_to_string),
				" %d", get_head(buffer)->ret_code);
	sprintf(buffer->element_to_string +
			strlen(buffer->element_to_string), "\n");
}

/**
 * @brief		Function, that starts iteration throw the log buffer.
 * 			It is used as the part of seq_ops.
 * @param [in] 	m	The seq_file where the log buffer data will be showed.
 * @param [in] 	pos	Isn't used.
 * @return		Returns pointer to buffer, NULL if buffer is empty.
 */
static void *emmcfs_log_buffer_start(struct seq_file *m, loff_t *pos)
{
	struct emmcfs_log_buffer *buffer;

	buffer = ((struct emmcfs_sb_info *)m->private)->buffer;

	if (emmcfs_check_buffer_empty(buffer))
		return NULL;
	return buffer;
}

/**
 * @brief		Function, that returns next log buffer element.
 * 			Used as the part of seq_ops.
 * @param [in] 	m	Isn't used.
 * @param [in] 	v	Function expects pointer to the log buffer here.
 * @param [in] 	pos	Isn't used.
 * @return		Returns pointer to buffer, NULL if buffer is empty.
 */
static void *emmcfs_log_buffer_next(struct seq_file *m, void *v, loff_t *pos)
{
	struct emmcfs_log_buffer *buffer;

	buffer = (struct emmcfs_log_buffer *)v;

	if (emmcfs_check_buffer_empty(buffer))
			return NULL;

	buffer->start = (buffer->start + 1) % BUFFER_LENGTH;
	return buffer;
}

/**
 * @brief		Function, that stops iteration throw the log buffer.
 * 			Used as the part of seq_ops.
 * @param [in] 	m	Isn't used.
 * @param [in] 	v	Isn't used.
 * @return	void
 */
static void emmcfs_log_buffer_stop(struct seq_file *m, void *v)
{
	struct emmcfs_log_buffer *buffer;
	char *str;

	buffer = ((struct emmcfs_sb_info *)m->private)->buffer;
	str = get_mutexes(buffer->sbi->nk);
	if (IS_ERR(str))
		EMMCFS_BUG();
	seq_printf(m, "%s", str);
	kfree(str);
}

/**
 * @brief		Shows one log buffer element. Used as the part
 *			of seq_ops.
 * @param [in] 	m	The seq_file, where element will be showed.
 * @param [in] 	v	Function expects pointer to the log buffer here.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_log_buffer_show(struct seq_file *m, void *v)
{
	struct emmcfs_log_buffer *buffer;
	buffer = (struct emmcfs_log_buffer *)v;
	emmcfs_element_to_string(buffer);
	seq_printf(m, "%s", buffer->element_to_string);
	return 0;
}

/**
 * @brief	Operations for iteration throw log buffer from proc file.
 */
static const struct seq_operations seq_ops = {
	.start = emmcfs_log_buffer_start,
	.next = emmcfs_log_buffer_next,
	.stop = emmcfs_log_buffer_stop,
	.show = emmcfs_log_buffer_show
};

/**
 * @brief		Opens proc file. Used as the part of log_buffer_fops.
 * @param [in] 	inode	The inode.
 * @param [in] 	file	File, that will be opened.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_log_buffer_open(struct inode *inode, struct file *file)
{
	struct seq_file *s;
	int result = seq_open(file, &seq_ops);
	s = (struct seq_file *)file->private_data;
	s->private = PROC_I(inode)->pde->data;
	return result;
}

/**
 * @brief	File operations for proc file.
 */
static const struct file_operations log_buffer_fops = {
    .owner	= THIS_MODULE,
    .open	= emmcfs_log_buffer_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release
};

/**
 * @brief	Creates proc entry for eMMCFS.
 * @return	Returns 0 on success, errno on failure.
 */
int emmcfs_dir_init(void)
{
	emmcfs_procdir_entry = proc_mkdir(EMMCFS_PROC_DIR_NAME, NULL);
	if (!emmcfs_procdir_entry)
		return -EINVAL;

	return 0;
}

/**
 * @brief		Destroys proc entry for eMMCFS.
 * @return	void
 */
void emmcfs_dir_exit(void)
{
	remove_proc_entry(EMMCFS_PROC_DIR_NAME, NULL);
}

/**
 * @brief		Builds log buffer.
 * @param [in] 	sbi	Runtime structure where log buffer will be placed.
 * @return		Returns 0 on success, errno on failure.
 */
int emmcfs_build_log_buffer(struct emmcfs_sb_info *sbi)
{
	struct proc_dir_entry *entry;
	int error = 0;
	struct emmcfs_log_buffer *buffer;
	char *devname = sbi->sb->s_id;

	buffer = kzalloc(sizeof(struct emmcfs_log_buffer), GFP_KERNEL);

	if (!buffer) {
		error = -ENOMEM;
		goto no_memory_for_buffer;
	}
	buffer->start = 0;
	buffer->end = 0;
	buffer->elements = (struct emmcfs_log_buffer_data *)
			vmalloc(sizeof(struct emmcfs_log_buffer_data) * BUFFER_LENGTH);

	if (!buffer->elements) {
		error = -ENOMEM;
		goto no_memory_for_buffer_elements;
	}

	buffer->element_to_string = kzalloc(sizeof(char) *
			EMMCFS_MAX_STAT_LENGTH, GFP_KERNEL);

	if (!buffer->element_to_string) {
		error = -ENOMEM;
		goto no_memory_for_print_string;
	}

	sbi->buffer = buffer;
	buffer->sbi = sbi;

	entry = proc_create_data(devname, 0, emmcfs_procdir_entry,
			&log_buffer_fops, sbi);
	if (!entry) {
		error = -EINVAL;
		goto cannot_create_proc_entry;
	}

	return 0;

cannot_create_proc_entry:
	kfree(buffer->element_to_string);
no_memory_for_print_string:
	vfree(buffer->elements);
no_memory_for_buffer_elements:
	kfree(buffer);
no_memory_for_buffer:
	return error;
}

/**
 * @brief		Destroys log buffer.
 * @param [in] 	sbi	Runtime structure where log buffer will be destroyed.
 * @return	void
 */
void emmcfs_destroy_log_buffer(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_log_buffer *buffer = sbi->buffer;
	char *devname = sbi->sb->s_id;

	sbi->buffer = NULL;
	vfree(buffer->elements);
	kfree(buffer->element_to_string);
	kfree(buffer);

	remove_proc_entry(devname, emmcfs_procdir_entry);
}

/**
 * @brief		Gets backtrace for eMMCFS device.
 * @param [in] 	sb	Superblock of the device for which backtrace will
 *			be got.
 * @return		Returns 0 on success, errno on failure.
 */
static int __get_backtrace(struct super_block *sb)
{
	char *str;

	if (!SB_HAS_EMMCFS_MAGIC(sb) || !EMMCFS_SB(sb)->buffer) {
		EMMCFS_ERR("Bad superblock!");
		return -EINVAL;
	}
	printk(KERN_ERR "[EMMCFS] backtrace for %s:\n",
			sb->s_bdev->bd_part->__dev.kobj.name);

#if defined(EMMCFS_GIT_BRANCH) && defined(EMMCFS_GIT_REV_HASH) && \
						defined(EMMCFS_VERSION)
	printk(KERN_ERR "[EMMCFS] version is \"%s\"", EMMCFS_VERSION);
	printk(KERN_ERR "[EMMCFS] git branch is \"%s\"", EMMCFS_GIT_BRANCH);
	printk(KERN_ERR "[EMMCFS] git revhash \"%.40s\"",
			EMMCFS_GIT_REV_HASH);
#endif
	printk(KERN_ERR "[EMMCFS] mkfs git branch is \"%s\"\n",
			(EMMCFS_RAw_SB(EMMCFS_SB(sb))->mkfs_git_branch));
	printk(KERN_ERR "[EMMCFS] mkfs git revhash \"%.40s\"\n",
			(EMMCFS_RAw_SB(EMMCFS_SB(sb))->mkfs_git_hash));

	printk(KERN_ERR "%s\n", get_table_title());
	while (emmcfs_log_buffer_next(NULL, EMMCFS_SB(sb)->buffer, NULL)) {
		emmcfs_element_to_string(EMMCFS_SB(sb)->buffer);
		printk(KERN_ERR "%s",
				EMMCFS_SB(sb)->buffer->element_to_string);
	}
	str = get_mutexes(EMMCFS_SB(sb)->buffer->sbi->nk);
	if (IS_ERR(str))
		return PTR_ERR(str);
	printk(KERN_ERR "%s", str);
	kfree(str);
	return 0;
}

/**
 * @brief	Gets bactrace for all mounted eMMCFS devices.
 * @return	Returns 0 on success, errno on failure.
 */
int emmcfs_get_backtrace(void)
{
	int ret = 0;
	struct sb_list *curr = &emmcfs_sb_list;
	do {
		if((ret = __get_backtrace(curr->element->sb)))
			break;
	}while ((curr = curr->next));
	return ret;
}

/**
 * @brief			Nesting keeper constructor.
 * @param [in,out] 	sbi	The eMMCFS superblock where nesting keeper
 *				will be placed.
 * @return			Returns 0 on success, errno on failure.
 */
int emmcfs_build_nesting_keeper(struct emmcfs_sb_info *sbi)
{
	int err = 0;
	int i = 0;
	struct emmcfs_nesting_keeper *nk;
	nk = kzalloc(sizeof(struct emmcfs_nesting_keeper), GFP_KERNEL);
	if (!nk) {
		err = -ENOMEM;
		goto err_exit;
	}
	for (i = 0; i < EMMCFS_MUTEX_COUNT; i++) {
		nk->locked_mutexes[i].pid = 0;
		nk->locked_mutexes[i].next = NULL;
	}
	spin_lock_init(&nk->nest_log_lock);
	nk->log_root = RB_ROOT;

	sbi->nk = nk;

err_exit:
	return err;
}

/**
 * @brief			Nest log destructor.
 * @param [in,out] 	root	Pointer to the RB-tree root that contains
 *				nest log.
 * @return		void
 */
static void destroy_nest_log(struct rb_root *root)
{
	struct rb_node *current_node;

		if (!root)
			return;

	current_node = rb_first(root);
	while (current_node) {
		struct rb_node *next_node = rb_next(current_node);
		struct emmcfs_nest_log *nest_log;

		nest_log = rb_entry(current_node,
				struct emmcfs_nest_log, node);

		rb_erase(current_node, root);
		kfree(nest_log);

		current_node = next_node;
	}
}

/**
 * @brief			Cleans pid list.
 * @param [in,out] 	l	Pid list that will be cleaned.
 * @return		void
 */
static void destroy_pid_list(struct pid_list *l)
{
	struct pid_list *curr = l;
	struct pid_list *tmp;
	curr->pid = 0;
	tmp = curr->next;
	curr->next = NULL;
	while ((curr = tmp)) {
		tmp = curr->next;
		kfree(curr);
	}
}

/**
 * @brief			Nesting keeper destructor.
 * @param [in,out] 	sbi	The eMMCFS superblock where nesting keeper
 *				will be erased.
 * @return		void
 */
void emmcfs_destroy_nesting_keeper(struct emmcfs_sb_info *sbi)
{
	int i;
	struct emmcfs_nesting_keeper *nk = sbi->nk;
	sbi->nk = NULL;
	destroy_nest_log(&nk->log_root);
	for (i = 0; i < EMMCFS_MUTEX_COUNT; i++)
		destroy_pid_list(&nk->locked_mutexes[i]);
	kfree(nk);
}

/**
 * @brief			Adds pid to pid list.
 * @param [in,out] 	l	List, where pid will be placed.
 * @param [in] 		pid	Pid that will be placed to pid list.
 * @return			Returns 0 on success, errno on failure.
 */
static void add_pid_to_list(struct pid_list *l, int pid)
{
	struct pid_list *curr = l;
	if(!curr->pid) {
		curr->pid = pid;
		return;
	}
	while (curr->next)
		curr = curr->next;

	curr->next = kzalloc(sizeof(struct pid_list), GFP_KERNEL);
	if (!curr->next)
		EMMCFS_BUG();
	curr->next->pid = pid;
	curr->next->next = NULL;
}

/**
 * @brief			Adds mutex lock info or increments nesting.
 * @param [in,out] 	sbi	Superblock info where nesting keeper is stored.
 * @param [in] 		flag	What mutex pid hold. If equals to 0
 *				increments nest.
 * @return			Returns 0 on success, errno on failure.
 */
int emmcfs_add_or_inc_data(struct emmcfs_sb_info *sbi, enum mutexes flag)
{
	struct emmcfs_nesting_keeper *nk = sbi->nk;
	struct rb_root *root = &sbi->nk->log_root;
	struct task_struct *current_task = current;
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	struct emmcfs_nest_log *this = NULL;

	while (*new) {
		this = container_of(*new, struct emmcfs_nest_log, node);
		parent = *new;
		if (current_task->pid < this->pid)
			new = &((*new)->rb_left);
		else if (current_task->pid > this->pid)
			new = &((*new)->rb_right);
		else {
			if (flag == EMMCFS_NEST)
				this->n++;
			else {
				add_pid_to_list(&nk->locked_mutexes[flag - 1],
					current_task->pid);
				nk->size[flag - 1]++;
			}
			return 0;
		}
	}

	this = kzalloc(sizeof(struct emmcfs_nest_log), GFP_KERNEL);
	if (!this)
		return -ENOMEM;

	this->pid = current->pid;
	this->n = 1;
	rb_link_node(&this->node, parent, new);
	rb_insert_color(&this->node, root);

	return 0;
}

/**
 * @brief			Removes pid from list.
 * @param [in,out] 	l	Pid list where from pid will be removed.
 * @param [in] 		pid	Pid that will be removed.
 * @return		void
 */
static void remove_pid_from_list(struct pid_list *l, int pid)
{
	struct pid_list *curr = l;

	while (curr->next) {
		if (curr->next->pid == pid) {
			struct pid_list *tmp;

			tmp = curr->next->next;
			kfree(curr->next);
			curr->next = tmp;
			break;
		}
		curr = curr->next;
	}
}

/**
 * @brief			Removes mutex lock info, or decrements nesting.
 * @param [in,out] 	sbi	Superblock info where nesting keeper is stored.
 * @param [in] 		flag	What mutex pid hold. If equal 0 decrements nest
 * @return			Returns 0 on success, errno on failure.
 */
int emmcfs_dec_data(struct emmcfs_sb_info *sbi, enum mutexes flag)
{
	struct emmcfs_nesting_keeper *nk = sbi->nk;
	struct rb_root *root = &sbi->nk->log_root;
	struct task_struct *current_task = current;
	struct rb_node **new = &(root->rb_node);
	struct emmcfs_nest_log *this = NULL;

	while (*new) {
		this = container_of(*new, struct emmcfs_nest_log, node);
		if (current_task->pid < this->pid)
			new = &((*new)->rb_left);
		else if (current_task->pid > this->pid)
			new = &((*new)->rb_right);
		else {
			if(flag == EMMCFS_NEST)
				this->n--;
			else {
				remove_pid_from_list(
						&nk->locked_mutexes[flag - 1],
						current_task->pid);
				nk->size[flag - 1]--;
			}
			return 0;
		}
	}
	return -EEXIST;
}

/**
 * @brief		Gets mutex lock info or current nesting for pid.
 * @param [in] 	nk	Nesting keeper where data is stored.
 * @param [in] 	flag	About what mutex get info. If 0 gets nesting.
 * @return		Returns 0 on success, errno on failure.
 */
int *emmcfs_get_data(struct emmcfs_nesting_keeper *nk, enum mutexes flag)
{
	struct rb_root *root = &nk->log_root;
	struct task_struct *current_task = current;
	struct rb_node **new = &(root->rb_node);
	struct emmcfs_nest_log *this = NULL;
	while (*new) {
		this = container_of(*new, struct emmcfs_nest_log, node);
		if (current_task->pid < this->pid)
			new = &((*new)->rb_left);
		else if (current_task->pid > this->pid)
			new = &((*new)->rb_right);
		else {
			if (flag == EMMCFS_NEST)
				return &this->n;
			else {
				struct pid_list *curr =
						&nk->locked_mutexes[flag - 1];
				int *mas;
				int i;

				mas = kzalloc(sizeof(int)*(nk->size[flag - 1]),
						GFP_KERNEL);
				if (!mas)
					return ERR_PTR(-ENOMEM);
				for (i = 0; i < nk->size[flag - 1]; i++) {
					mas[i] = curr->pid;
					curr = curr->next;
				}
				return mas;
			}
		}
	}
	return ERR_PTR(-EEXIST);
}
#else
/* Dummy functions for logging is off case*/
int emmcfs_function_add(struct emmcfs_log_buffer *buffer, const char *name,
		int8_t flag, int ret) {return 0;}
int emmcfs_dir_init(void) {return 0;}
void emmcfs_dir_exit(void) { }
int emmcfs_get_backtrace(void){return 0;}
int emmcfs_init_sb_list(void) {return 0;}
int emmcfs_add_sb_to_list(struct emmcfs_sb_info *sbi) {return 0;}
int emmcfs_remove_from_list(struct emmcfs_sb_info *sbi) {return 0;}
int emmcfs_build_nesting_keeper(struct emmcfs_sb_info *sbi) {return 0;}
void emmcfs_destroy_nesting_keeper(struct emmcfs_sb_info *sbi) {}
int emmcfs_add_or_inc_data(struct emmcfs_sb_info *sbi, enum mutexes flag)
								{return 0;}
int emmcfs_dec_data(struct emmcfs_sb_info *sbi, enum mutexes flag) {return 0;}
int *emmcfs_get_data(struct emmcfs_nesting_keeper *nk, enum mutexes flag) {return 0;}
#endif
