# CCEP 2010 Platform
obj-$(CONFIG_ARCH_SDP1001)	+= ccep_soc/sdp1001/
obj-$(CONFIG_MACH_GRIFFIN)	+= platform/griffin/
obj-$(CONFIG_ARCH_SDP1001FPGA)	+= ccep_soc/sdp1001fpga/
obj-$(CONFIG_MACH_FPGA_GENOA)	+= platform/fpga_genoa/

obj-$(CONFIG_ARCH_SDP1002)	+= ccep_soc/sdp1002/
obj-$(CONFIG_MACH_DALMA)	+= platform/dalma/

obj-$(CONFIG_ARCH_SDP1004)	+= ccep_soc/sdp1004/
obj-$(CONFIG_ARCH_SDP1004FPGA)	+= ccep_soc/sdp1004fpga/

obj-$(CONFIG_MACH_FIRENZE)	+= platform/firenze/
obj-$(CONFIG_MACH_FPGA_FIRENZE)	+= platform/fpga_firenze/

# CCEP 2011 Platform
obj-$(CONFIG_ARCH_SDP1103)	+= ccep_soc/sdp1103/
obj-$(CONFIG_ARCH_SDP1106)	+= ccep_soc/sdp1106/
obj-$(CONFIG_ARCH_SDP1105)	+= ccep_soc/sdp1105/

obj-$(CONFIG_MACH_FLAMENGO)	+= platform/flamengo/
obj-$(CONFIG_MACH_SANTOS)	+= platform/santos/
obj-$(CONFIG_MACH_VICTORIA)	+= platform/victoria/

obj-$(CONFIG_ARCH_SDP1106FPGA)	+= ccep_soc/sdp1106fpga/
obj-$(CONFIG_MACH_FPGA_FLAMENGO) += platform/fpga_flamengo/

obj-$(CONFIG_ARCH_SDP1105FPGA)	+= ccep_soc/sdp1105fpga/
obj-$(CONFIG_MACH_FPGA_VICTORIA) += platform/fpga_victoria/

obj-$(CONFIG_ARCH_SDP1103FPGA)	+= ccep_soc/sdp1103fpga/
obj-$(CONFIG_MACH_FPGA_SANTOS)	+= platform/fpga_santos/

obj-$(CONFIG_ARCH_SDP1114)	+= ccep_soc/sdp1114/
obj-$(CONFIG_MACH_ECHOE)	+= platform/echoe/

# CCEP 2012 Platform
obj-$(CONFIG_ARCH_SDP1202)	+= ccep_soc/sdp1202/
obj-$(CONFIG_MACH_FOXAP)	+= platform/foxap/

obj-$(CONFIG_ARCH_SDP1202FPGA)	+= ccep_soc/sdp1202fpga/
obj-$(CONFIG_MACH_FPGA_FOXAP)	+= platform/fpga_foxap/

obj-$(CONFIG_ARCH_SDP1207FPGA)	+= ccep_soc/sdp1207fpga/
obj-$(CONFIG_MACH_FPGA_FOXB)	+= platform/fpga_foxb/

obj-$(CONFIG_ARCH_SDP1207)	+= ccep_soc/sdp1207/
obj-$(CONFIG_MACH_FOXB)	+= platform/foxb/


# Multi Core Support
obj-$(CONFIG_SMP)		+= platsmp.o headsmp.o hotplug.o
obj-$(CONFIG_HAVE_ARM_TWD)	+= localtimer.o
obj-$(CONFIG_HAVE_ARM_GT)	+= generictimer.o

# DVFS Support
obj-$(CONFIG_CPU_FREQ) += cpufreq.o

# Patch Errata
obj-$(CONFIG_SDP_ARM_A8_ERRATA_586324)	+= v7a8_cache.o

