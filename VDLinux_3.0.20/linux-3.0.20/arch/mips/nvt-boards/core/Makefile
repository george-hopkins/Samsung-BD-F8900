#
# Carsten Langgaard, carstenl@mips.com
# Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
#
# Copyright (C) 2008 Wind River Systems, Inc.
#   written by Ralf Baechle <ralf@linux-mips.org>
#
#obj-y				:= nvt-amon.o nvt-cmdline.o 
obj-y				:= nvt-cmdline.o \
				   nvt-display.o nvt-init.o nvt-int.o \
				   nvt-memory.o nvt-time.o #\
#				   nvt-wbflush.o

obj-$(CONFIG_EARLY_PRINTK)	+= nvt-console.o
obj-$(CONFIG_PCI)		+= nvt-pci.o

# FIXME FIXME FIXME
obj-$(CONFIG_MIPS_MT_SMTC)	+= nvt-smtc.o

