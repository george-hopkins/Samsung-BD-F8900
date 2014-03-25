/*
 * arch/arm/mach-ccep/include/mach/irqs.h
 *
 * Copyright (C) 2010 SAMSUNG ELECTRONICS  
 * Author : tukho.kim@samsung.com
 *
 */

#ifndef __SDP_IRQS_H
#define __SDP_IRQS_H

#if defined(CONFIG_ARCH_SDP92)
#include "irqs-sdp92.h"
#elif defined(CONFIG_ARCH_SDP1001)
#include "irqs-sdp1001.h"
#elif defined(CONFIG_ARCH_SDP1001FPGA)
#include "irqs-sdp1001fpga.h"
#elif defined(CONFIG_ARCH_SDP1002)
#include "irqs-sdp1002.h"
#elif defined(CONFIG_ARCH_SDP1004)
#include "irqs-sdp1004.h"
#elif defined(CONFIG_ARCH_SDP1004FPGA)
#include "irqs-sdp1004fpga.h"
#elif defined(CONFIG_ARCH_SDP1103)
#include "irqs-sdp1103.h"
#elif defined(CONFIG_ARCH_SDP1105)
#include "irqs-sdp1105.h"
#elif defined(CONFIG_ARCH_SDP1106)
#include "irqs-sdp1106.h"
#elif defined(CONFIG_ARCH_SDP1103FPGA)
#include "irqs-sdp1103fpga.h"
#elif defined(CONFIG_ARCH_SDP1105FPGA)
#include "irqs-sdp1105fpga.h"
#elif defined(CONFIG_ARCH_SDP1106FPGA)
#include "irqs-sdp1106fpga.h"
#elif defined(CONFIG_ARCH_SDP1202)
#include "irqs-sdp1202.h"
#elif defined(CONFIG_ARCH_SDP1202FPGA)
#include "irqs-sdp1202fpga.h"
#else
#error 
#endif

#endif /* __SDP_IRQS_H */

