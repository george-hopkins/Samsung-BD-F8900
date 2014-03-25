/* linux/arch/arm/mach-ccep/include/plat/asv.h
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * SDP - Adoptive Support Voltage Header file
 *
 */

#ifndef __ASM_ARCH_ASV_H
#define __ASM_ARCH_ASV_H

//#include <mach/regs-pmu.h>
//#include <mach/regs-pmu5.h>

//#include <plat/cpu.h>

#define JUDGE_TABLE_END		NULL

#define LOOP_CNT			10

#define MAX_MP_COUNT		2

extern unsigned int sdp_result_of_asv;
extern unsigned int sdp_asv_stored_result;
extern unsigned int sdp_asv_gpu_ids;


struct asv_judge_table {
	unsigned int ids_limit; /* IDS value to decide group of target */
	unsigned int tmcb_limit; /* TMCB value to decide group of target */
};

struct asv_ids_volt_table {
	unsigned int ids; /* ids value */
	int volt; /* micro volt */
};

struct sdp_asv_info {
	unsigned long long ap_pkg_id;	/* fused value for ap package */
	unsigned long long mp_pkg_id[MAX_MP_COUNT];	/* fused value for mp package */
//	unsigned int ids_offset;		/* ids_offset of chip */
//	unsigned int ids_mask;			/* ids_mask of chip */
	unsigned int cpu_ids;			/* cpu ids value of chip */
	unsigned int cpu_tmcb;			/* cpu tmcb value of chip */
	unsigned int gpu_ids;			/* gpu ids value */
	unsigned int mp_ids[MAX_MP_COUNT];		/* mp ids value */
	unsigned int mp_tmcb[MAX_MP_COUNT];		/* mp tmcb value */
	int (*check_vdd_arm)(void);		/* check vdd_arm value, this function is selectable */
//	int (*pre_clock_init)(void);		/* clock init function to get hpm */
//	int (*pre_clock_setup)(void);		/* clock setup function to get hpm */
	/* specific get ids function */
	int (*get_cpu_ids)(struct sdp_asv_info *asv_info);
	/* specific get tmcb function */
	int (*get_cpu_tmcb)(struct sdp_asv_info *asv_info);
	/* specific get cpu ids function*/
	int (*get_gpu_ids)(struct sdp_asv_info *asv_info);
	/* store into some repository to send result of asv */
	int (*store_result)(struct sdp_asv_info *asv_info);
};

struct sdp_asv_info * get_sdp_asv_info(void);

#if defined(CONFIG_ARCH_SDP1202)
int sdp_asv_mp_training(unsigned int mp_index);
#endif

#endif /* __ASM_ARCH_ASV_H */

