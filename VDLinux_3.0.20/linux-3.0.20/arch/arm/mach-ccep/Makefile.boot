ifeq ($(CONFIG_SDP_DISCONTIGMEM_SUPPORT),y)
zreladdr-y	:= 0x60008000
params_phys-y	:= 0x60000100
initrd_phys-y	:= 0x60800000
endif

ifeq ($(CONFIG_SDP_SPARSEMEM_SUPPORT),y)
zreladdr-y	:= 0x60008000
params_phys-y	:= 0x60000100
initrd_phys-y	:= 0x60800000
endif

ifeq ($(CONFIG_SDP_SINGLE_DDR_A),y)
zreladdr-y      := 0x60008000
params_phys-y   := 0x60000100
initrd_phys-y   := 0x60800000
endif

ifeq ($(CONFIG_ARCH_SDP1106), y)
zreladdr-y      := 0x40008000
params_phys-y   := 0x40000100
initrd_phys-y   := 0x40800000
endif

ifeq ($(CONFIG_ARCH_SDP1202FPGA), y)
zreladdr-y	:= 0x40008000
params_phys-y	:= 0x40000100
initrd_phys-y	:= 0x40800000
endif

ifeq ($(CONFIG_ARCH_SDP1202), y)
zreladdr-y	:= 0x40008000
params_phys-y	:= 0x40000100
initrd_phys-y	:= 0x40800000
endif

ifeq ($(CONFIG_ARCH_SDP1207), y)
zreladdr-y	:= 0x40008000
params_phys-y	:= 0x40000100
initrd_phys-y	:= 0x40800000
endif

ifeq ($(CONFIG_SDP_SINGLE_DDR_B),y)
zreladdr-y      := 0x80008000
params_phys-y   := 0x80000100
initrd_phys-y   := 0x80800000
endif

ifeq ($(CONFIG_SDP_SINGLE_DDR_C),y)
zreladdr-y	:= 0xA0008000
params_phys-y	:= 0xA0000100
initrd_phys-y	:= 0xA0800000
endif

