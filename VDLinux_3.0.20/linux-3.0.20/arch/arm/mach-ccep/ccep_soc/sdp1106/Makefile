obj-$(CONFIG_ARCH_SDP1106)	+= sdp1106.o sdp1106_clock.o sdp1106_gpio.o sdp1106_misc.o

# DVFS Support
obj-$(CONFIG_CPU_FREQ_SDP1106) += sdp1106_cpufreq.o
ifeq ($(CONFIG_SDP_THERMAL),y)
obj-$(CONFIG_ARCH_SDP1106) += sdp1106_tmu.o
endif
