#ifndef __LOWMEMORYKILLER_H
#define __LOWMEMORYKILLER_H
#include <linux/oom.h>
#include <linux/ioctl.h>

#define DEV_NAME_DEF	"dev/lmk"
#define LMK_MINOR	242

/*
 * Params for ioctl call
 *
 * REG_MANAGER_IOCTL:
 *	pid - process id
 *	val - ignored
 * SET_KILL_PRIO_IOCTL:
 *	pid - process id
 *	val - priority for killing app in lowmem condition.
 *	      higher val is for not important application.
 *	      three values are alowed:
 *	      passive - 6
 *	      active - 5
 *	      OOM_DISABLE - protect app.
 */
struct lmk_ioctl {
	int pid;
	int val;
};

#define LMK_MAGIC       'l'

#define SET_KILL_PRIO_IOCTL  _IOW(LMK_MAGIC, 1, struct lmk_ioctl)
#define REG_MANAGER_IOCTL _IOW(LMK_MAGIC, 2, struct lmk_ioctl)

#endif
