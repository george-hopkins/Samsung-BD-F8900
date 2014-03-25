/*
 * arch/arm/mach-ccep/include/mach/platform.h
 *
 *  Copyright (C) 2003-2010 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef __SDP_PLATFORM_H
#define __SDP_PLATFORM_H

#if defined(CONFIG_MACH_AQUILA)
#   include <mach/aquila.h>
#elif defined(CONFIG_MACH_GRIFFIN)
#	include <mach/griffin.h>
#elif defined(CONFIG_MACH_FPGA_GENOA)
#	include <mach/fpga_genoa.h>
#elif defined(CONFIG_MACH_DALMA)
#	include <mach/dalma.h>
#elif defined(CONFIG_MACH_FIRENZE)
#	include <mach/firenze.h>
#elif defined(CONFIG_MACH_FPGA_FIRENZE)
#	include <mach/fpga_firenze.h>
#elif defined(CONFIG_MACH_FLAMENGO)
#	include <mach/flamengo.h>
#elif defined(CONFIG_MACH_FPGA_FLAMENGO)
#	include <mach/fpga_flamengo.h>
#elif defined(CONFIG_MACH_SANTOS)
#	include <mach/santos.h>
#elif defined(CONFIG_MACH_FPGA_SANTOS)
#	include <mach/fpga_santos.h>
#elif defined(CONFIG_MACH_VICTORIA)
#	include <mach/victoria.h>
#elif defined(CONFIG_MACH_FPGA_VICTORIA)
#	include <mach/fpga_victoria.h>
#elif defined(CONFIG_MACH_FOXAP)
#	include <mach/foxap.h>
#elif defined(CONFIG_MACH_FPGA_FOXAP)
#	include <mach/fpga_foxap.h>
#else
#   error "Please Choose platform\n"
#endif

#endif// __SDP_PLATFORM_H
