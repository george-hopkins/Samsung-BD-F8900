ifeq ($(SUB_TARGET_ARCH),Linux)
IWU_SRC = $(TOP)/applications/iwu/csr_bt_iwu_linux.c
else
IWU_SRC = $(TOP)/applications/iwu/csr_bt_iwu_windows.c
endif
