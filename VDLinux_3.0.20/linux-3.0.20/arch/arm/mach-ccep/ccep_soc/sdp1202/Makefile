
obj-$(CONFIG_ARCH_SDP1202)	+= sdp1202.o sdp1202_clock.o sdp1202_misc.o sdp1202_mpintr.o
obj-$(CONFIG_SDP_GPIO)		+= sdp1202_gpio.o

# DVFS Support
obj-$(CONFIG_CPU_FREQ_SDP1202) += sdp1202_cpufreq.o sdp1202_asv.o sdp1202_asv_mptrain.o
ifeq ($(CONFIG_SDP_THERMAL),y)
obj-$(CONFIG_ARCH_SDP1202) += sdp1202_tmu.o
endif
