#ifndef _SDP_GPIO_BOARD_H_
#define _SDP_GPIO_BOARD_H_

#include <linux/kernel.h>
#include <linux/spinlock.h>

#define SDP_GPIO_MAX_PINS	8

enum sdp_gpio_pstate {
	SDP_GPIO_PULL_OFF = 0,
	SDP_GPIO_PULL_DN,
	SDP_GPIO_PULL_RESERVED,
	SDP_GPIO_PULL_UP,
};

struct sdp_gpio_pull {
	u32			reg_offset;	/* offset from GPIO_BASE */
	u32			reg_shift;
	enum sdp_gpio_pstate	state;		/* saved context */
};

struct sdp_gpio_reg {
	u32	con;	
	u32	wdat;
	u32	rdat;
};

struct sdp_gpio_port {
	u32				portno;
	u32				npins;		
	u32				reg_offset;	/* offset from GPIO_BASE */
	
	spinlock_t			lock_port;
	struct sdp_gpio_reg __iomem	*reg;
	u32	 			control;	/* saved context */
	
	struct sdp_gpio_pull		pins[SDP_GPIO_MAX_PINS];
};

int sdp_gpio_add_device (u32 addr, size_t reg_len, struct sdp_gpio_port *ports, int nports);

#endif
