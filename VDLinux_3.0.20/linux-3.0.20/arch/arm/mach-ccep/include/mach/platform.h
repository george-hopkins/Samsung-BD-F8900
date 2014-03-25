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
#   include <mach/aquila/aquila.h>
#elif defined(CONFIG_MACH_GRIFFIN)
#	include <mach/griffin/griffin.h>
#elif defined(CONFIG_MACH_FPGA_GENOA)
#	include <mach/fpga_genoa/fpga_genoa.h>
#elif defined(CONFIG_MACH_DALMA)
#	include <mach/dalma/dalma.h>
#elif defined(CONFIG_MACH_FIRENZE)
#	include <mach/firenze/firenze.h>
#elif defined(CONFIG_MACH_FPGA_FIRENZE)
#	include <mach/fpga_firenze/fpga_firenze.h>
#elif defined(CONFIG_MACH_FLAMENGO)
#	include <mach/flamengo/flamengo.h>
#elif defined(CONFIG_MACH_FPGA_FLAMENGO)
#	include <mach/fpga_flamengo/fpga_flamengo.h>
#elif defined(CONFIG_MACH_SANTOS)
#	include <mach/santos/santos.h>
#elif defined(CONFIG_MACH_FPGA_SANTOS)
#	include <mach/fpga_santos/fpga_santos.h>
#elif defined(CONFIG_MACH_VICTORIA)
#	include <mach/victoria/victoria.h>
#elif defined(CONFIG_MACH_FPGA_VICTORIA)
#	include <mach/fpga_victoria/fpga_victoria.h>
#elif defined(CONFIG_MACH_FOXAP)
#	include <mach/foxap/foxap.h>
#elif defined(CONFIG_MACH_FPGA_FOXAP)
#	include <mach/fpga_foxap/fpga_foxap.h>
#elif defined(CONFIG_MACH_FPGA_FOXB)
#	include <mach/fpga_foxb/fpga_foxb.h>
#elif defined(CONFIG_MACH_ECHOE)
#	include <mach/echoe/echoe.h>
#elif defined(CONFIG_MACH_FOXB)
#	include <mach/foxb/foxb.h>
#else
#   error "Please Choose platform\n"
#endif

#endif// __SDP_PLATFORM_H
