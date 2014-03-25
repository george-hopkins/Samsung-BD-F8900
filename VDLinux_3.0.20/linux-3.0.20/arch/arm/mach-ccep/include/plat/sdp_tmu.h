/* linux/arch/arm/mach-ccep/include/plat/sdp-tmu.h
 *
 * Copyright 2012 Samsung Electronics Co., Ltd.
 *      http://www.samsung.com/
 *
 * Header file for sdp tmu support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __SDP_TMU_H
#define __SDP_TMU_H

#define TMU_SAVE_NUM   10

#define TMU_INV_FREQ_LEVEL	(0xFFFFFFFF)

#define TMU_1ST_THROTTLE	(1)
#define TMU_2ND_THROTTLE	(2)
#define TMU_3RD_THROTTLE	(3)

enum tmu_state_t {
	TMU_STATUS_INIT = 0,
	TMU_STATUS_ZERO,
	TMU_STATUS_NORMAL,
	TMU_STATUS_1ST,
	TMU_STATUS_2ND,
	TMU_STATUS_3RD,
};

/*
 * struct temperature_params have values to manange throttling, tripping
 * and other software safety control
 */
struct temperature_params {
	unsigned int start_zero_throttle;
	unsigned int stop_zero_throttle;
	unsigned int stop_1st_throttle;
	unsigned int start_1st_throttle;
	unsigned int stop_2nd_throttle;
	unsigned int start_2nd_throttle;
	unsigned int stop_3rd_throttle;
	unsigned int start_3rd_throttle;
};

struct cpufreq_params {
	unsigned int limit_1st_throttle;
	unsigned int limit_2nd_throttle;
	unsigned int limit_3rd_throttle;
};

struct gpufreq_params {
	unsigned int limit_1st_throttle;
	unsigned int limit_2nd_throttle;
	unsigned int limit_3rd_throttle;
};

struct temp_compensate_params {
	unsigned int arm_volt; /* temperature compensated voltage */
};

struct memory_params {
	unsigned int rclk;
	unsigned int period_bank_refresh;
};

struct tmu_config {
	unsigned char mode;
	unsigned char slope;
	unsigned int sampling_rate;
	unsigned int monitoring_rate;
};

struct sdp_platform_tmu {
	struct temperature_params ts;
	struct cpufreq_params cpufreq;
	struct gpufreq_params gpufreq;
	struct temp_compensate_params temp_compensate;
	struct memory_params mp;
	struct tmu_config cfg;
};

struct sdp_tmu_info {
	struct device   *dev;

	int     id;
	char *sdp_name;

	void __iomem    *tmu_base;
	struct resource *ioarea;
	int irq;

	int	tmu_state;
	int tmu_prev_state;
	unsigned int last_temperature;

	unsigned int cpufreq_level_1st_throttle;
	unsigned int cpufreq_level_2nd_throttle;
	unsigned int cpufreq_level_3rd_throttle;
	unsigned int gpufreq_level_1st_throttle;
	unsigned int gpufreq_level_2nd_throttle;
	unsigned int gpufreq_level_3rd_throttle;

	struct delayed_work monitor;
	struct delayed_work polling;

	unsigned int monitor_period;
	unsigned int sampling_rate;
	unsigned int reg_save[TMU_SAVE_NUM];

	int fusing_diffval;
	int (*gpu_freq_limit)(int freq); /* gpu frequency limit function */	
	int (*gpu_freq_limit_free)(void); /* gpu frequency limit free function */
	int (*gpu_freq_emergency_limit)(void); /* gpu frequency emergency limit function */
	int (*gpu_freq_emergency_limit_free)(void); /* gpu frequency emergency limit free */
};

struct sdp_tmu *sdp_tmu_get_platdata(void);
int sdp_tmu_get_irqno(int num);
struct sdp_tmu_info* sdp_tmu_get_info(void);
extern struct platform_device sdp_device_tmu;
#endif /* __SDP_TMU_H */

