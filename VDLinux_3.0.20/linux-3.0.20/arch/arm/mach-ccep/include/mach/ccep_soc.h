/***********************************************************************************
 *	
 *	file : ccep_soc.h
 *
 * ********************************************************************************/

/***********************************************************************************
 * 2012.04.06, tukho.kim, created 
 *
 * ********************************************************************************/


#ifndef __CCEP_SOC_H__
#define __CCEP_SOC_H__

#if defined(CONFIG_MACH_FPGA_GENOA)
#	include <mach/fpga_genoa/sdp1001fpga.h>
#elif defined(CONFIG_MACH_DALMA)
#	include <mach/dalma/sdp1002.h>
#elif defined(CONFIG_MACH_FLAMENGO)
#	include <mach/flamengo/sdp1106.h>
#elif defined(CONFIG_MACH_FPGA_FLAMENGO)
#	include <mach/fpga_flamengo/sdp1106fpga.h>
#elif defined(CONFIG_MACH_SANTOS)
#	include <mach/santos/sdp1103.h>
#elif defined(CONFIG_MACH_FOXAP)
#	include <mach/foxap/sdp1202.h>
#elif defined(CONFIG_MACH_FPGA_FOXAP)
#	include <mach/fpga_foxap/sdp1202fpga.h>
#elif defined(CONFIG_MACH_ECHOE)
#	include <mach/echoe/sdp1114.h>

/*  BD Defined */
#elif defined(CONFIG_MACH_FIRENZE)
#	include <mach/firenze/sdp1004.h>
#elif defined(CONFIG_MACH_FPGA_FIRENZE)
#	include <mach/fpga_firenze/sdp1004fpga.h>
#elif defined(CONFIG_MACH_VICTORIA)
#	include <mach/victoria/sdp1105.h>
#elif defined(CONFIG_MACH_FPGA_VICTORIA)
#	include <mach/fpga_victoria/sdp1105fpga.h>
#elif defined(CONFIG_MACH_FPGA_FOXB)
#	include <mach/fpga_foxb/sdp1207fpga.h>
#elif defined(CONFIG_MACH_FOXB)
#	include <mach/foxb/sdp1207.h>
#else
# 	error "Not define CCEP SOC"
#endif 


#endif /* __CCEP_SOC_H__ */
