/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Display routines for display messages in MIPS boards ascii display.
 */

#include <linux/module.h>

#if 1
// Added by Mstar
// Add a variable to set panel init status for fast logo function.
static int sys_pnl_init_status = 0;

void sys_set_pnl_init_status(int status)
{
    sys_pnl_init_status = status;
    return;
}
EXPORT_SYMBOL(sys_set_pnl_init_status);

int sys_get_pnl_init_status(void)
{
    return sys_pnl_init_status;
}
EXPORT_SYMBOL(sys_get_pnl_init_status);
#endif

#if 0 /* not used */
void mips_display_message(const char *str)
{
}

void scroll_display_message(unsigned long data)
{
}

void mips_scroll_message(void)
{
}
#endif
