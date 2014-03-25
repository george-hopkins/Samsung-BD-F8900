/*
 * arch/arm/mach-ccep/include/mach/irqs.h
 *
 * Copyright (C) 2010 SAMSUNG ELECTRONICS  
 * Author : tukho.kim@samsung.com
 *
 */

#ifndef __SDP_IRQS_H
#define __SDP_IRQS_H

#if defined(CONFIG_ARCH_SDP1001)
#include "genoas/irqs-sdp1001.h"
#elif defined(CONFIG_ARCH_SDP1001FPGA)
#include "fpga_genoa/irqs-sdp1001fpga.h"
#elif defined(CONFIG_ARCH_SDP1002)
#include "dalma/irqs-sdp1002.h"
#elif defined(CONFIG_ARCH_SDP1004)
#include "firenze/irqs-sdp1004.h"
#elif defined(CONFIG_ARCH_SDP1004FPGA)
#include "fpga_firenze/irqs-sdp1004fpga.h"
#elif defined(CONFIG_ARCH_SDP1103)
#include "santos/irqs-sdp1103.h"
#elif defined(CONFIG_ARCH_SDP1105)
#include "victoria/irqs-sdp1105.h"
#elif defined(CONFIG_ARCH_SDP1106)
#include "flamengo/irqs-sdp1106.h"
#elif defined(CONFIG_ARCH_SDP1103FPGA)
#include "fpga_santos/irqs-sdp1103fpga.h"
#elif defined(CONFIG_ARCH_SDP1105FPGA)
#include "fpga_victoria/irqs-sdp1105fpga.h"
#elif defined(CONFIG_ARCH_SDP1106FPGA)
#include "fpga_flamengo/irqs-sdp1106fpga.h"
#elif defined(CONFIG_ARCH_SDP1202)
#include "foxap/irqs-sdp1202.h"
#elif defined(CONFIG_ARCH_SDP1202FPGA)
#include "fpga_foxap/irqs-sdp1202fpga.h"
#elif defined(CONFIG_ARCH_SDP1207FPGA)
#include "fpga_foxb/irqs-sdp1207fpga.h"
#elif defined(CONFIG_ARCH_SDP1114)
#include "echoe/irqs-sdp1114.h"
#elif defined(CONFIG_MACH_FOXB)
#   include <mach/foxb/irqs-sdp1207.h>
#else
#error "Not defined IRQ."
#endif

#endif /* __SDP_IRQS_H */

